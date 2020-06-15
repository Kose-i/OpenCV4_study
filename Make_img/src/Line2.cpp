#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
  cv::Mat img = cv::Mat::zeros(125, 125, CV_8U);

  for (auto y = 0;y < img.cols;++y) {
    for (auto x = 0;x < img.rows;++x) {
      img.at<unsigned char>(y,x) = x*2;
    }
  }
  cv::imshow("Make-img", img);
  cv::imwrite("../img/Line/Line2.png", img);
  cv::waitKey(0);
}
