#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
  //cv::Mat img = cv::Mat::zeros(125, 125, CV_8U);
  cv::Mat img = cv::Mat::zeros(20, 20, CV_8U);
  std::vector<int> X {10, 1, 10, 2, 10, 3, 10, 4, 10, 5, 10, 8, 10, 20};
  std::vector<int> x_step;
  int c {};
  for (auto i = 0;i < X.size();++i) {
    c+= X[i];
    if (i%2==0)continue;
    for (auto j = 0;j<X[i];++j) {
      x_step.push_back(c+j);
    }
  }
  for (const auto& e : x_step) {
    for (auto y = 0;y < img.cols;++y) {
      for (auto x = 0;x < img.rows;++x) {
        if (x == e) {
          img.at<unsigned char>(y,x) = 255;
        }
      }
    }
  }
  cv::imshow("Make-img", img);
  cv::imwrite("../img/Line/Line1.png", img);
  cv::waitKey(0);
}
