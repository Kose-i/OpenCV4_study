#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <utility>
#include <vector>

using Pair = std::pair<int, int>;

#include "align_img.hpp"
//#include "get_checkbox.hpp"

//bool operator<(Box& l, Box& r) {
//  if (l.lt.y==r.lt.y) {
//    return (l.lt.x<r.lt.x);
//  }
//  return (l.lt.y < r.lt.y);
//}

int main(int argc, char** argv) {

  const std::string template_file_name {"../dataset/Template.jpg"};
  cv::Mat template_img = cv::imread(template_file_name, CV_LOAD_IMAGE_GRAYSCALE);

  std::string check_file_name {"../dataset/Target7.jpg"};

  cv::Mat check_img = cv::imread(check_file_name, 0);
  if (template_img.empty() || check_img.empty()) {
    std::cout << "empty\n";
    return -1;
  }

  cv::Mat align_check_img = align_img::align(template_img, check_img);

  cv::imshow("template", template_img);
  cv::imshow(check_file_name+"-img", align_check_img);
  while(cv::waitKey(0) != 'q');
}
