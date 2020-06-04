#include <opencv2/opencv.hpp>
#include <string>

std::string src_img_name {"../img/lenna.jpg"};


int main(int argc, char** argv) {
  cv::Mat src_img = cv::imread(src_img_name);
  if (src_img.empty()) {
    std::cerr << src_img_name << " is not found\n";
    return -1;
  }
  cv::imshow("src-img", src_img); 
  cv::imwrite("../img/Laplacian/src-img.png", src_img);
  cv::waitKey(0);

  cv::Mat dst_img = src_img;

  cv::Mat filter = cv::Mat::zeros(3, 3, CV_64F);

  filter.at<double>(0,0) = 0;filter.at<double>(0,1) = 1;filter.at<double>(0,2) = 0;
  filter.at<double>(1,0) = 1;filter.at<double>(1,1) =-4;filter.at<double>(1,2) = 1;
  filter.at<double>(2,0) = 0;filter.at<double>(2,1) = 1;filter.at<double>(2,2) = 0;

  cv::filter2D(src_img, dst_img, src_img.type(), filter);

  cv::imshow("dst-img", dst_img);
  cv::imwrite("../img/Laplacian/dst-img.png", dst_img);
  cv::waitKey(0);
}
