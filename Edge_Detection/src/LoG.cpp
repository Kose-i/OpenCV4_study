#include <opencv2/opencv.hpp>
#include <string>

std::string src_img_name {"../img/lenna.jpg"};

int main(int argc, char** argv) {
  cv::Mat src_img = cv::imread(src_img_name, cv::IMREAD_GRAYSCALE);
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

  cv::Mat LoG_img = cv::Mat::zeros(gaussian_img.rows, gaussian_img.cols, CV_64F);
  cv::Mat Laplacian_filter = cv::Mat::zeros(3, 3, CV_64F);

  Laplacian_filter.at<double>(0,0) = 0;Laplacian_filter.at<double>(0,1) = 1;Laplacian_filter.at<double>(0,2) = 0;
  Laplacian_filter.at<double>(1,0) = 1;Laplacian_filter.at<double>(1,1) =-4;Laplacian_filter.at<double>(1,2) = 1;
  Laplacian_filter.at<double>(2,0) = 0;Laplacian_filter.at<double>(2,1) = 1;Laplacian_filter.at<double>(2,2) = 0;

  std::cout << LoG_img.rows << '-' << LoG_img.cols << '\n';
  std::cout << gaussian_img.rows << '-' << gaussian_img.cols << '\n';

  for (auto y = 0;y < gaussian_img.rows;++y) {
    for (auto x = 0;x < gaussian_img.cols;++x) {
      //double val = LoG_img.at<double>(x,y);
//      if (val==0) {
//        LoG_img.at<double>(x,y) = 0;
//        continue;
//      }
      double a1,a2,a3, b1,b2,b3, c1,c2,c3;
      //a1 = (x!=0&&y!=0)?gaussian_img.at<double>(x-1,y-1):0;
      a1 = (x!=0&&y!=0)?gaussian_img.at<double>(y-1,x-1):0;
      //b1 = (y!=0)?gaussian_img.at<double>(x, y-1):0;
      b1 = (y!=0)?gaussian_img.at<double>(y-1, x):0;
      //c1 = (y!=0&&x!=gaussian_img.cols-1)?gaussian_img.at<double>(x+1,y-1):0;
      c1 = (y!=0&&x!=gaussian_img.cols-1)?gaussian_img.at<double>(y-1,x+1):0;
      //a2 = (x!=0)?gaussian_img.at<double>(x-1,y):0;
      a2 = (x!=0)?gaussian_img.at<double>(y,x-1):0;
      //b2 = gaussian_img.at<double>(x, y);
      b2 = gaussian_img.at<double>(y,x);
      //c2 = (x!=gaussian_img.cols-1)?gaussian_img.at<double>(x+1,y):0;
      c2 = (x!=gaussian_img.cols-1)?gaussian_img.at<double>(y,x+1):0;
      //a3 = (y!=gaussian_img.rows-1&&x!=0)?gaussian_img.at<double>(x-1,y+1):0;
      a3 = (y!=gaussian_img.rows-1&&x!=0)?gaussian_img.at<double>(y+1,x-1):0;
      //b3 = (y!=gaussian_img.rows-1)?gaussian_img.at<double>(x, y+1):0;
      b3 = (y!=gaussian_img.rows-1)?gaussian_img.at<double>(y+1,x):0;
      //c3 = (y!=gaussian_img.rows-1&&x!=gaussian_img.cols-1)?gaussian_img.at<double>(x+1,y+1):0;
      c3 = (y!=gaussian_img.rows-1&&x!=gaussian_img.cols-1)?gaussian_img.at<double>(y+1,x+1):0;
      LoG_img.at<double>(y,x)= b1;
      LoG_img.at<double>(y,x)+= a2;
      LoG_img.at<double>(y,x)+= b2*(-4);
      LoG_img.at<double>(y,x)+= c2;
      LoG_img.at<double>(y,x)+= b3;
      if (LoG_img.at<double>(y,x)<0)std::cout << "-";
      //LoG_img.at<double>(y,x) = a1*Laplacian_filter.at<double>(0,0);
      //LoG_img.at<double>(y,x)+= b1*Laplacian_filter.at<double>(1,0);
      //LoG_img.at<double>(y,x)+= c1*Laplacian_filter.at<double>(2,0);
      //LoG_img.at<double>(y,x)+= a2*Laplacian_filter.at<double>(0,1);
      //LoG_img.at<double>(y,x)+= b2*Laplacian_filter.at<double>(1,1);
      //LoG_img.at<double>(y,x)+= c2*Laplacian_filter.at<double>(2,1);
      //LoG_img.at<double>(y,x)+= a3*Laplacian_filter.at<double>(0,2);
      //LoG_img.at<double>(y,x)+= b3*Laplacian_filter.at<double>(1,2);
      //LoG_img.at<double>(y,x)+= c3*Laplacian_filter.at<double>(2,2);
    }
  }
  //cv::filter2D(gaussian_img, LoG_img, gaussian_img.type(), Laplacian_filter);

  cv::imshow("LoG-img", LoG_img);
  cv::imwrite("../img/LoG/LoG-img.png", LoG_img);
  cv::waitKey(0);

  int LoG_rows = LoG_img.rows;
  int LoG_cols = LoG_img.cols;

  cv::Mat LoG_img_a = cv::Mat::zeros(LoG_rows, LoG_cols, CV_8U);

  for (auto y = 0;y < LoG_rows;++y) {
    for (auto x = 0;x < LoG_cols;++x) {
      double val = LoG_img.at<double>(x,y);
      if (val==0) {
        LoG_img_a.at<unsigned char>(x,y) = 255;
        continue;
      }
      double a1,a2,a3, b1,b2,b3, c1,c2,c3;
      a1 = (x!=0&&y!=0)?src_img.at<double>(x-1,y-1):0;
      b1 = (y!=0)?src_img.at<double>(x, y-1):0;
      c1 = (y!=0&&x!=src_img.cols-1)?src_img.at<double>(x+1,y-1):0;
      a2 = (x!=0)?src_img.at<double>(x-1,y):0;
      b2 = src_img.at<double>(x, y);
      c2 = (x!=src_img.cols-1)?src_img.at<double>(x+1,y):0;
      a3 = (y!=src_img.rows-1&&x!=0)?src_img.at<double>(x-1,y+1):0;
      b3 = (y!=src_img.rows-1)?src_img.at<double>(x, y+1):0;
      c3 = (y!=src_img.rows-1&&x!=src_img.cols-1)?src_img.at<double>(x+1,y+1):0;
      if (val*a1<0||val*a2<0||val*a3<0||val*b1<0||val*b2<0||val*b3<0||val*c1<0||val*c2<0||val*c3<0) {
        LoG_img_a.at<unsigned char>(x,y) = 255;
      }
    }
  }
  cv::imshow("LoG-img-a", LoG_img_a);
  cv::imwrite("../img/LoG/LoG-img-a.png", LoG_img_a);
  cv::waitKey(0);
}
