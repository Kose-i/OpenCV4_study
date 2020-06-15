#include <opencv2/opencv.hpp>
#include <string>

std::string src_img_name {"../img/lenna.jpg"};


int main(int argc, char** argv) {
  cv::Mat src_img = cv::imread(src_img_name);
  if (src_img.empty()) {
    std::cerr << src_img_name << " is not found\n";
    return -1;
  }
  std::cout << src_img.rows << ' ' << src_img.cols << '\n';
  cv::imshow("src-img", src_img); 
  cv::imwrite("../img/Canny/src-img.png", src_img);
  cv::waitKey(0);

  cv::Mat dst_img = src_img;

  int threshold1 {80};
  int threshold2 {200};

  std::stringstream ss;
  ss << "../img/Canny/";
  ss << threshold1 << '-' << threshold2 << "dst-img.png";

  cv::Canny(src_img, dst_img, threshold1, threshold2);

  cv::imshow("dst-img", dst_img);
  //cv::imwrite("../img/Canny/dst-img.png", dst_img);
  cv::imwrite(ss.str(), dst_img);
  cv::waitKey(0);
}
