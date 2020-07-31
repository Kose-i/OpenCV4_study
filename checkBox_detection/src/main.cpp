#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>

#include "align_img.hpp"
#include "get_checkbox.hpp"

bool operator<(Box& l, Box& r) {
  return (l.lt.y < r.lt.y)?true:(l.lt.x<r.lt.x);
}

int main(int argc, char** argv) {

  const std::string template_file_name {"../dataset/Template.jpg"};
  const std::string check_file_name {"../dataset/Target2.jpg"};
  constexpr int diff_eps {10};//align?10:1

  cv::Mat template_img = cv::imread(template_file_name, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat check_img = cv::imread(check_file_name, CV_LOAD_IMAGE_GRAYSCALE);

  cv::Mat align_check_img = align_img::align(template_img, check_img);

  cv::Mat diff_img = check_img;
  for (auto y = 0;y < diff_img.cols;++y) {
    for (auto x = 0;x < diff_img.rows;++x) {
      int a = static_cast<int>(align_check_img.at<unsigned char>(x,y));
      int b = static_cast<int>(template_img.at<unsigned char>(x,y));
      if (std::abs(a-b) < diff_eps) {
        diff_img.at<unsigned char>(x, y) = 0;
      } else {
        diff_img.at<unsigned char>(x, y) = 255;
      }
    }
  }
  cv::Mat median_diff_img = diff_img;
  medianBlur(diff_img, median_diff_img, 3);//(3,3);

  //std::vector<std::vector<cv::Point>> output_contours;
  //cv::findContours(median_diff_img, output_contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
  //
  //median_diff_img = cv::Scalar::all(0);
  //cv::drawContours(median_diff_img, output_contours, -1, cv::Scalar::all(255));
  //cv::imshow("median_diff-img", median_diff_img);

  std::vector<Box> checkbox_pos = get_checkbox_pos(template_img);

  // std::cout << checkbox_pos.size() << '\n'; // 46
  std::vector<int> checkbox_inner(checkbox_pos.size(), 0);

  std::sort(checkbox_pos.begin(), checkbox_pos.end());
  std::cout << checkbox_pos.size() << '\n';

  for (auto i = 0;i < checkbox_pos.size();++i) {
    Box tmp = checkbox_pos[i];
    int ans {};
    for (auto y = tmp.lt.y;y < tmp.rb.y;++y) {
      for (auto x = tmp.lt.x;x < tmp.rb.x;++x) {
        if (diff_img.at<unsigned char>(y,x)==255) ++ans;
      }
    }
    //std::cout << tmp.lt.x << ' ' << tmp.lt.y << ' ' << tmp.rb.x << ' ' << tmp.rb.y << '\n';
    cv::rectangle(diff_img, tmp.lt, tmp.rb, cv::Scalar(255));//draw rectangle
    checkbox_inner[i] = ans;
  }
  for (const auto& e : checkbox_inner) {
    std::cout << ' ' << e;
  }
  std::cout << '\n';

  cv::imshow("diff-img", diff_img);
  while(cv::waitKey(0)!='q');
}
