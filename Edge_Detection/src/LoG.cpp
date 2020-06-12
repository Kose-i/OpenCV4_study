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

std::vector<cv::Mat> make_sobel_filters() {
  std::vector<cv::Mat> Sobel_filters(8);
  for (int i = 0;i < 8;++i) Sobel_filters[i] = cv::Mat::zeros(3,3,CV_64F);

  Sobel_filters[0].at<double>(0,0) =-1;Sobel_filters[0].at<double>(0,1) = 0;Sobel_filters[0].at<double>(0,2) = 1;
  Sobel_filters[0].at<double>(1,0) =-2;Sobel_filters[0].at<double>(1,1) = 0;Sobel_filters[0].at<double>(1,2) = 2;
  Sobel_filters[0].at<double>(2,0) =-1;Sobel_filters[0].at<double>(2,1) = 0;Sobel_filters[0].at<double>(2,2) = 1;
  for (auto i = 1;i<8;++i) {
    Sobel_filters[i].at<double>(0,0)=Sobel_filters[i-1].at<double>(1,0);
    Sobel_filters[i].at<double>(1,0)=Sobel_filters[i-1].at<double>(2,0);
    Sobel_filters[i].at<double>(2,0)=Sobel_filters[i-1].at<double>(2,1);
    Sobel_filters[i].at<double>(2,1)=Sobel_filters[i-1].at<double>(2,2);
    Sobel_filters[i].at<double>(2,2)=Sobel_filters[i-1].at<double>(1,2);
    Sobel_filters[i].at<double>(1,2)=Sobel_filters[i-1].at<double>(0,2);
    Sobel_filters[i].at<double>(0,2)=Sobel_filters[i-1].at<double>(0,1);
    Sobel_filters[i].at<double>(0,1)=Sobel_filters[i-1].at<double>(0,0);
  }
  return Sobel_filters;
}

int main(int argc, char** argv) {
  cv::Mat src_img = cv::imread(src_img_name, cv::IMREAD_GRAYSCALE);//8U
  if (src_img.empty()) {
    std::cerr << src_img_name << " is not found\n";
    return -1;
  }
  //cv::imshow("src-img", src_img); 
  //cv::imwrite("../img/LoG/src-img.png", src_img);
  //cv::waitKey(0);

  cv::Mat gaussian_img = cv::Mat::zeros(src_img.rows, src_img.cols, CV_8U);
  cv::Mat Gaussian_filter = cv::Mat::zeros(3, 3, CV_64F);

  Gaussian_filter.at<double>(0,0) = 0.0625;Gaussian_filter.at<double>(0,1) = 0.125;Gaussian_filter.at<double>(0,2) = 0.0625;
  Gaussian_filter.at<double>(1,0) = 0.125; Gaussian_filter.at<double>(1,1) = 0.25; Gaussian_filter.at<double>(1,2) = 0.125;
  Gaussian_filter.at<double>(2,0) = 0.0625;Gaussian_filter.at<double>(2,1) = 0.125;Gaussian_filter.at<double>(2,2) = 0.0625;

  cv::filter2D(src_img, gaussian_img, CV_8U, Gaussian_filter);
  //cv::imshow("Gaussian-img", gaussian_img);
  //cv::imwrite("../img/LoG/Gaussian-img.png", gaussian_img);
  //cv::waitKey(0);

  cv::Mat LoG_img = cv::Mat::zeros(gaussian_img.rows, gaussian_img.cols, CV_8U);
  cv::Mat Laplacian_filter = cv::Mat::zeros(3, 3, CV_64F);

  Laplacian_filter.at<double>(0,0) = 0;Laplacian_filter.at<double>(0,1) = 1;Laplacian_filter.at<double>(0,2) = 0;
  Laplacian_filter.at<double>(1,0) = 1;Laplacian_filter.at<double>(1,1) =-4;Laplacian_filter.at<double>(1,2) = 1;
  Laplacian_filter.at<double>(2,0) = 0;Laplacian_filter.at<double>(2,1) = 1;Laplacian_filter.at<double>(2,2) = 0;

  cv::filter2D(gaussian_img, LoG_img, gaussian_img.type(), Laplacian_filter);
  //cv::imshow("LoG-img", LoG_img);
  //cv::imwrite("../img/LoG/LoG-img.png", LoG_img);
  //cv::waitKey(0);

  cv::Mat LoG_img_a = cv::Mat::zeros(LoG_img.rows, LoG_img.cols, CV_8U);
  constexpr unsigned char thread_a {10};

  for (auto y = 0;y < LoG_img.rows;++y) {
    for (auto x = 0;x < LoG_img.cols;++x) {
      bool now_point = (LoG_img.at<unsigned char>(x,y)<=thread_a)?true:false;
      bool a1,a2,a3, b1,b2,b3, c1,c2,c3;
      a1 = (x!=0&&y!=0)?((LoG_img.at<unsigned char>(x-1,y-1)<=thread_a)?true:false):now_point;
      b1 = (y!=0)?((LoG_img.at<unsigned char>(x,y-1)<=thread_a)?true:false):now_point;
      c1 = (y!=0&&x!=LoG_img.cols-1)?((LoG_img.at<unsigned char>(x+1,y-1)<=thread_a)?true:false):now_point;
      a2 = (x!=0)?((LoG_img.at<unsigned char>(x-1,y)<=thread_a)?true:false):now_point;
      b2 = now_point;
      c2 = (x!=src_img.cols-1)?((LoG_img.at<unsigned char>(x+1,y)<=thread_a)?true:false):now_point;
      a3 = (y!=src_img.rows-1&&x!=0)?((LoG_img.at<unsigned char>(x-1,y+1)<=thread_a)?true:false):now_point;
      b3 = (y!=src_img.rows-1)?((LoG_img.at<unsigned char>(x,y+1)<=thread_a)?true:false):now_point;
      c3 = (y!=src_img.rows-1&&x!=src_img.cols-1)?((LoG_img.at<unsigned char>(x+1,y+1)<=thread_a)?true:false):now_point;
      auto is_not_same = [](bool a1, bool a2, bool a3, bool a4){return !((a1==a2)&&(a2==a3)&&(a3==a4));};
      if (is_not_same(a1, b1, a2, b2) || is_not_same(b1,c1, b2, c2) || is_not_same(a2, b2, a3, b3) || is_not_same(b2, c2, b3, c3)) {
        LoG_img_a.at<unsigned char>(x,y) = 255;
      } else {
        LoG_img_a.at<unsigned char>(x,y) = 0;
      }
    }
  }
//  cv::imshow("LoG-img-a", LoG_img_a);
//  cv::imwrite("../img/LoG/LoG-img-a.png", LoG_img_a);
  //cv::imshow("a-result", LoG_img_a);
  //cv::imwrite("../img/LoG/LoG-img-a-result.png", LoG_img_a);
  //cv::waitKey(0);
  
  //TODO
  cv::Mat LoG_img_b = cv::Mat::zeros(LoG_img.rows, LoG_img.cols, CV_8U);
  {
    std::vector<cv::Mat> Sobel_filters = make_sobel_filters();
    for (const auto& e : Sobel_filters) {
      cv::Mat tmp_img = cv::Mat::zeros(gaussian_img.rows, gaussian_img.cols, gaussian_img.type());
      cv::filter2D(gaussian_img, tmp_img, gaussian_img.type(), Sobel_filters[0]);
      for (auto y = 0;y < tmp_img.rows;++y) {
        for (auto x = 0;x < tmp_img.cols;++x) {
          LoG_img_b.at<unsigned char>(x,y) = std::max(tmp_img.at<unsigned char>(x,y), LoG_img_b.at<unsigned char>(x,y));
        }
      }
    }

    unsigned char thread_Sobel {50};
    for (auto y = 0;y < LoG_img_b.rows;++y) {
      for (auto x = 0;x < LoG_img_b.cols;++x) {
        if (LoG_img_a.at<unsigned char>(x,y)==255 && LoG_img_b.at<unsigned char>(x,y) > thread_Sobel) {
          LoG_img_b.at<unsigned char>(x,y) = 255;
        } else {
          LoG_img_b.at<unsigned char>(x,y) = 0;
        }
      }
    }
  }
  cv::imshow("b-result", LoG_img_b);
  cv::waitKey(0);
}
