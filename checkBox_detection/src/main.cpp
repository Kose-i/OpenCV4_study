#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "align_img.hpp"

int main(int argc, char** argv) {

  std::string template_file_name {};
  std::string check_file_name {};
  cv::Mat template_img = cv::imread(template_file_name, CV_IMREAD_GRAY);
  cv::Mat check_img = cv::imread(check_file_name, CV_IMREAD_GRAY);

  cv::Mat align_check_img = align_img::align(template_file_name, check_img);

  constexpr unsigned char diff_eps {10};

  cv::Mat diff_img = check_img;
  for (auto y = 0;y < diff_img.cols;++y) {
    for (auto x = 0;x < diff_img.rows;++x) {
      if (std::abs(align_check_img.at<unsigned char>(x,y) - template_file_name.at<unsigned char>(x,y)) < diff_eps) {
        diff_img.at<unsigned char>(x, y) = 0;
      } else {
        diff_img.at<unsigned char>(x, y) = 255;
      }
    }
  }
}
