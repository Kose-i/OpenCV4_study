#include <opencv2/opencv.hpp>
#include <math.h>
#include <cmath>

int main(int argc, char** argv) {

  int RGB_R{30}, RGB_G{50}, RGB_B{80};
  std::cout << "RGB_R:" << RGB_R << " G:" << RGB_G << " B:" << RGB_B << '\n';

  //{
  //  cv::Mat frame = cv::Mat::ones(640, 480, CV_8UC3)*cv::Scalar(RGB_R, RGB_G, RGB_B);
  //  cv::namedWindow("RGB-frame");
  //  cv::imshow("RGB-frame", frame);
  //  while ( true ) {
  //    if (cv::waitKey(1) == 'q') break;
  //  }
  //}

  int YIQ_Y, YIQ_I, YIQ_Q{};
  YIQ_Y = 0.299*RGB_R + 0.587*RGB_G + 0.114*RGB_B;
  YIQ_I = 0.596*RGB_R - 0.274*RGB_G - 0.322*RGB_B;
  YIQ_Q = 0.211*RGB_R - 0.522*RGB_G + 0.311*RGB_B;
  std::cout << "Y:" << YIQ_Y << " I:" << YIQ_I << " Q:" << YIQ_Q << '\n';

  double HSI_H, HSI_S, HSI_I{};
  {
    double I_max, I_min {};
    I_max = (RGB_R > RGB_G)?((RGB_R > RGB_B)?RGB_R: RGB_B):((RGB_G > RGB_B)?RGB_G:RGB_B);
    I_min = (RGB_R < RGB_G)?((RGB_R < RGB_B)?RGB_R: RGB_B):((RGB_G < RGB_B)?RGB_G:RGB_B);
    if ( I_max == I_min ) {
      std::cerr << "I_max == I_min\n";
      return -1;
    }
    HSI_I = I_max;
    HSI_S = (I_max - I_min)/I_max;
    double r = (I_max - RGB_R) / (I_max - I_min);
    double g = (I_max - RGB_G) / (I_max - I_min);
    double b = (I_max - RGB_B) / (I_max - I_min);
    if (RGB_R == I_max) HSI_H = M_PI*(b - g)/3;
    if (RGB_G == I_max) HSI_H = M_PI*(2 + r - b)/3;
    if (RGB_B == I_max) HSI_H = M_PI*(4 + g - r)/3;
  }
  std::cout << "H:" << HSI_H << " S:" << HSI_S << " I:" << HSI_I << '\n';

  double XYZ_X, XYZ_Y, XYZ_Z {};
  XYZ_X = 100*(0.3933*std::pow((RGB_R/255, 2.2)) + 0.3651*std::pow((RGB_G/255, 2.2)) + 0.1903*std::pow(RGB_B/255, 2.2));
  XYZ_y = 100*(0.2123*std::pow((RGB_R/255, 2.2)) + 0.7010*std::pow((RGB_G/255, 2.2)) + 0.0858*std::pow(RGB_B/255, 2.2));
  XYZ_Z = 100*(0.0182*std::pow((RGB_R/255, 2.2)) + 0.1117*std::pow((RGB_G/255, 2.2)) + 0.9570*std::pow(RGB_B/255, 2.2));
  std::cout << "X:" << XYZ_X << " Y:" << XYZ_Y << " Z:" << XYZ_Z << '\n';
}
