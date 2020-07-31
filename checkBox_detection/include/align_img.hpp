#ifndef align_img_hpp
#define align_img_hpp

#include <iostream>
#include <opencv2/opencv.hpp>

namespace align_img {
  cv::Mat align(const cv::Mat& template_img, const cv::Mat& img);
}

#endif
