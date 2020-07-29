#include <iiostream>
#include <opencv2/opencv.hpp>

namespace align_img {
  cv::Mat align_img(const cv::Mat& template_img, const cv::Mat& img);
}
