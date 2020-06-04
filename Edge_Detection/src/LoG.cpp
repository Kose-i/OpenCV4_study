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
  cv::imwrite("../img/LoG/src-img.png", src_img);
  cv::waitKey(0);

  cv::Mat gaussian_img = src_img;
  cv::Mat Gaussian_filter = cv::Mat::zeros(3, 3, CV_64F);

  Gaussian_filter.at<double>(0,0) = 0.0625;Gaussian_filter.at<double>(0,1) = 0.125;Gaussian_filter.at<double>(0,2) = 0.0625;
  Gaussian_filter.at<double>(1,0) = 0.125;Gaussian_filter.at<double>(1,1) =0.25;Gaussian_filter.at<double>(1,2) = 0.125;
  Gaussian_filter.at<double>(2,0) = 0.0625;Gaussian_filter.at<double>(2,1) = 0.125;Gaussian_filter.at<double>(2,2) = 0.0625;

  cv::filter2D(src_img, gaussian_img, src_img.type(), Gaussian_filter);
  cv::imshow("Gaussian-img", gaussian_img);
  cv::imwrite("../img/LoG/Gaussian-img.png", gaussian_img);

  cv::Mat LoG_img = src_img;
  cv::Mat Laplacian_filter = cv::Mat::zeros(3, 3, CV_64F);

  Laplacian_filter.at<double>(0,0) = 0;Laplacian_filter.at<double>(0,1) = 1;Laplacian_filter.at<double>(0,2) = 0;
  Laplacian_filter.at<double>(1,0) = 1;Laplacian_filter.at<double>(1,1) =-4;Laplacian_filter.at<double>(1,2) = 1;
  Laplacian_filter.at<double>(2,0) = 0;Laplacian_filter.at<double>(2,1) = 1;Laplacian_filter.at<double>(2,2) = 0;

  cv::filter2D(gaussian_img, LoG_img, gaussian_img.type(), Laplacian_filter);

  cv::imshow("LoG-img", LoG_img);
  cv::imwrite("../img/LoG/LoG-img.png", LoG_img);
  cv::waitKey(0);
}
