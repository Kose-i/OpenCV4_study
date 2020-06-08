#include <opencv2/opencv.hpp>
#include <string>

std::string src_img_name {"../img/lenna.jpg"};
constexpr int epsilon {1};

cv::Mat senkei(cv::Mat img) {
  double Min, Max;
  cv::minMaxLoc(img, &Min, &Max);
  img -= Min;
  cv::Mat dist_img;
  img.convertTo(dist_img, CV_8U, 255.0/(Max-Min+epsilon));
  return dist_img;
}

int main(int argc, char** argv) {
  cv::Mat src_img = cv::imread(src_img_name);
  if (src_img.empty()) {
    std::cerr << src_img_name << " is not found\n";
    return -1;
  }
//  cv::imshow("src-img", src_img); 
//  cv::imwrite("../img/DoG/src-img.png", src_img);
  cv::waitKey(0);

  double sigma_e {3};//3//10
  double sigma_i {sigma_e*1.6};

  cv::Mat Gaussian_sigma_e;
  cv::Mat Gaussian_sigma_i;

  GaussianBlur(src_img, Gaussian_sigma_e, cv::Size(3, 3), sigma_e);
  GaussianBlur(src_img, Gaussian_sigma_i, cv::Size(3, 3), sigma_i);
  //GaussianBlur(src_img, Gaussian_sigma_e, cv::Size(1, 1), 0);
  //GaussianBlur(src_img, Gaussian_sigma_i, cv::Size(3, 3), 0);

//  cv::imshow("Gaussian_sigma_e", Gaussian_sigma_e);
//  //cv::imwrite("../img/Dog/Gaussian_sigma_e.png", Gaussian_sigma_e);
//  cv::imwrite("Gaussian_sigma_e.png", Gaussian_sigma_e);

//  cv::imshow("Gaussian_sigma_i", Gaussian_sigma_i);
//  cv::imwrite("../img/Dog/Gaussian_sigma_i.png", Gaussian_sigma_i);

  cv::Mat DoG_img = Gaussian_sigma_e - Gaussian_sigma_i;
  //cv::Mat DoG_img = src_img - Gaussian_sigma_e;
  DoG_img = senkei(DoG_img);

  cv::imshow("DoG-img", DoG_img);
  std::stringstream ss;
  //ss << "../img/DoG/" << Gaussian_sigma_e << '_' << Gaussian_sigma_i << "DoG.png";
  //ss << "DoG.png";
  cv::imwrite("../img/DoG/DoG-img3.png", DoG_img);
  std::cout << sigma_e << ' ' << sigma_i << '\n';
  //cv::imwrite(ss.str(), DoG_img);
  cv::waitKey(0);
}
