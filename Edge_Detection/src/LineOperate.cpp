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
  cv::imwrite("../img/LineOperate/src-img.png", src_img);
  cv::waitKey(0);

  cv::Mat dst_img = src_img;

  cv::Mat filter = cv::Mat::zeros(3, 3, CV_64F);

  filter.at<double>(0,0) = -0.5;filter.at<double>(0,1) = 1;filter.at<double>(0,2) = -0.5;
  filter.at<double>(1,0) = -0.5;filter.at<double>(1,1) = 1;filter.at<double>(1,2) = -0.5;
  filter.at<double>(2,0) = -0.5;filter.at<double>(2,1) = 1;filter.at<double>(2,2) = -0.5;

  cv::filter2D(src_img, dst_img, src_img.type(), filter);

  cv::imshow("dst-img", dst_img);
  cv::imwrite("../img/LineOperate/dst-img.png", dst_img);
  cv::waitKey(0);
  std::cout << dst_img.rows << "*" << dst_img.cols << '\n';
  for (auto y = 0;y < dst_img.rows;++y) {
    for (auto x = 0;x < dst_img.cols;++x) {
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

      if ((b1+b2+b3>a1+a2+a3)&&(b1+b2+b3>c1+c2+c3)) continue;
      //dst_img.at<unsigned char>(y,x) = 0;
      dst_img.at<unsigned char>(x,y) = 0;
    }
  }
  cv::imshow("dst-img3", dst_img);
  cv::imwrite("../img/LineOperate/dst-img3.png", dst_img);
  cv::waitKey(0);

  filter.at<double>(0,0) = -0.5;filter.at<double>(0,1) = -0.5;filter.at<double>(0,2) = -0.5;
  filter.at<double>(1,0) = 1;filter.at<double>(1,1) = 1;filter.at<double>(1,2) = 1;
  filter.at<double>(2,0) = -0.5;filter.at<double>(2,1) = -0.5;filter.at<double>(2,2) = -0.5;

  cv::filter2D(src_img, dst_img, src_img.type(), filter);

  cv::imshow("dst-img", dst_img);
  cv::imwrite("../img/LineOperate/dst-img2.png", dst_img);
  cv::waitKey(0);
}
