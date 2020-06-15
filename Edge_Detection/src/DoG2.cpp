#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void on_make_sigmaE(int,void*);
void on_make_sigmaI(int,void*);
int sigma_e=100, sigma_i=100;
void on_make_sigmaE(int,void*) {
  sigma_e = max(0,sigma_e);
  sigma_e = min(800,sigma_e);
  setTrackbarPos("Sigma_e","Object Detection",sigma_e);
}

void on_make_sigmaI(int,void*) {
  sigma_i = max(0,sigma_i);
  sigma_i = min(800,sigma_i);
  setTrackbarPos("Sigma_i","Object Detection",sigma_i);
}
constexpr int epsilon {1};

cv::Mat senkei(cv::Mat img) {
  double Min, Max;
  cv::minMaxLoc(img, &Min, &Max);
  img -= Min;
  cv::Mat dist_img;
  img.convertTo(dist_img, CV_8U, 255.0/(Max-Min+epsilon));
  return dist_img;
}

int main(int argc, char *argv[]) {
  //cv::Mat src_img = cv::imread("../img/lenna.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat src_img = cv::imread("../../Make_img/img/Line/Line1.png", CV_LOAD_IMAGE_GRAYSCALE);
  //cv::Mat src_img = cv::imread("../../Make_img/img/Line/Line2.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (src_img.empty()) {
    std::cerr << "is not opened\n";
    return -1;
  }
  cv::namedWindow("Object Detection",WINDOW_NORMAL);
  cv::createTrackbar("Sigma_e","Object Detection",&sigma_e,800,on_make_sigmaE);
  cv::createTrackbar("Sigma_i","Object Detection",&sigma_i,800,on_make_sigmaI);
  while(true) {
    double sigma_e_double {sigma_e/100.0};//3//10
    double sigma_i_double {sigma_i/100.0};

    cv::Mat Gaussian_sigma_e;
    cv::Mat Gaussian_sigma_i;

    GaussianBlur(src_img, Gaussian_sigma_e, cv::Size(7, 7), sigma_e_double);
    GaussianBlur(src_img, Gaussian_sigma_i, cv::Size(7, 7), sigma_i_double);

    cv::Mat DoG_img = Gaussian_sigma_e - Gaussian_sigma_i;
    //cv::Mat DoG_img = cv::Mat::zeros(Gaussian_sigma_e.rows, Gaussian_sigma_e.cols, CV_8U);
    //for (auto y = 0;y < Gaussian_sigma_e.cols;++y) {
    //  for (auto x = 0;x < Gaussian_sigma_e.rows;++x) {
    //    if (Gaussian_sigma_e.at<unsigned char>(x,y) < Gaussian_sigma_i.at<unsigned char>(x,y)) {
    //      DoG_img.at<unsigned char>(x,y) = 255;
    //    }
    //  }
    //}
    //DoG_img = senkei(DoG_img);
    cv::imshow("gaussian-sigma_e", Gaussian_sigma_e);
    cv::imshow("gaussian-sigma_i", Gaussian_sigma_i);
    cv::imshow("DoG_img", DoG_img);
    char key_push = cv::waitKey(1);
    if (key_push=='q') {
      break;
    } else if (key_push == 's') {
      cv::imwrite("../img/DoG2/DoG3.png", DoG_img);
    }
  }
  return 0;
}
