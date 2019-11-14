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
  {
    double tmp_r = std::pow((RGB_R/255.0), 2.2);
    double tmp_g = std::pow((RGB_G/255.0), 2.2);
    double tmp_b = std::pow((RGB_B/255.0), 2.2);
    XYZ_X = 100*(0.3933*tmp_r + 0.3651*tmp_g + 0.1903*tmp_b);
    XYZ_Y = 100*(0.2123*tmp_r + 0.7010*tmp_g + 0.0858*tmp_b);
    XYZ_Z = 100*(0.0182*tmp_r + 0.1117*tmp_g + 0.9570*tmp_b);
  }
  std::cout << "X:" << XYZ_X << " Y:" << XYZ_Y << " Z:" << XYZ_Z << '\n';

  double xyz_x{}, xyz_y{}, xyz_z{};
  xyz_x = XYZ_X/(XYZ_X+XYZ_Y+XYZ_Z);
  xyz_y = XYZ_Y/(XYZ_X+XYZ_Y+XYZ_Z);
  xyz_z = XYZ_Z/(XYZ_X+XYZ_Y+XYZ_Z);
  std::cout << "x:" << xyz_x << " y:" << xyz_y << " z:" << xyz_z << '\n';

  double CIE_Lab_L{}, CIE_Lab_a{}, CIE_Lab_b{};
  {
    double X_0{108.87}, Y_0{100.0}, Z_0{35.59};
    double tmp_x = std::pow(XYZ_X/X_0, 1/3.0);
    double tmp_y = std::pow(XYZ_Y/Y_0, 1/3.0);
    double tmp_z = std::pow(XYZ_Z/Z_0, 1/3.0);
    CIE_Lab_L = 116*tmp_y - 16;
    CIE_Lab_a = 500*(tmp_x - tmp_y);
    CIE_Lab_b = 200*(tmp_y - tmp_z);
  }
  std::cout << "L*:" << CIE_Lab_L << " a*:" << CIE_Lab_a << " b*:" << CIE_Lab_b << '\n';

  int CMY_C{}, CMY_M{}, CMY_Y{};
  CMY_C = 255 - RGB_R;
  CMY_M = 255 - RGB_G;
  CMY_Y = 255 - RGB_B;
  std::cout << "C:" << CMY_C << " M:" << CMY_M << " Y:" << CMY_Y << '\n';

  int CMYK_C{}, CMYK_M{}, CMYK_Y{}, CMYK_K{};
  {
    int k = 20;
    auto ucr = [](int k, const char& color){
      if(color=='c') return k;
      if(color=='m') return k;
      if(color=='y') return k;
    };
    auto bg = [](int k) {
      return k;
    };
    int c = 255 - RGB_R;
    int m = 255 - RGB_G;
    int y = 255 - RGB_B;
    CMYK_C = (255 < ((0 > c - ucr(k, 'c'))?0:c-ucr(k, 'c'))?255:((0 > c - ucr(k, 'c'))?0:c-ucr(k, 'c')));
    CMYK_M = (255 < ((0 > m - ucr(k, 'm'))?0:m-ucr(k, 'm'))?255:((0 > m - ucr(k, 'm'))?0:m-ucr(k, 'm')));
    CMYK_Y = (255 < ((0 > y - ucr(k, 'y'))?0:y-ucr(k, 'y'))?255:((0 > y - ucr(k, 'y'))?0:y-ucr(k, 'y')));
    CMYK_K = (255 < ((0 > bg(k))?0:bg(k))?255:((0 > bg(k))?0:bg(k)));
  }
  std::cout << "C:" << CMYK_C << " M:" << CMYK_M << " Y:" << CMYK_Y << " K:" << CMYK_K << '\n';

  double YCbCr_Y{}, YCbCr_Cb{}, YCbCr_Cr{};
}
