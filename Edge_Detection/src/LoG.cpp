#include <opencv2/opencv.hpp>
#include <string>

constexpr double thread_log_a {0.01};
constexpr double thread_Sobel {10};
constexpr int thread_a {30};
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

  cv::filter2D(gaussian_img, LoG_img, gaussian_img.type(), Laplacian_filter);
  cv::imshow("LoG-img", LoG_img);
  cv::imwrite("../img/LoG/LoG-img.png", LoG_img);
  cv::waitKey(0);

  int LoG_rows = LoG_img.rows;
  int LoG_cols = LoG_img.cols;

  cv::Mat LoG_img_a = senkei(LoG_img);
  cv::Mat LoG_img_a_result = cv::Mat::zeros(LoG_rows, LoG_cols, CV_8U);

  for (auto y = 0;y < LoG_rows;++y) {
    for (auto x = 0;x < LoG_cols;++x) {
      double val = LoG_img_a.at<unsigned char>(x,y);
      if (val>=thread_a) {
        LoG_img_a_result.at<unsigned char>(x,y) = 255;
//        continue;
      } else {
        continue;
      }
      //if (x!=0&&y!=0)LoG_img_a_result.at<double>(x-1,y-1)=255;
      //if (y!=0)LoG_img_a_result.at<double>(x, y-1)=0;
      //if (y!=0&&x!=LoG_img_a_result.cols-1)LoG_img_a_result.at<double>(x+1,y-1)=0;
      //if (x!=0)LoG_img_a_result.at<double>(x-1,y)=0;
      //if (x!=LoG_img_a_result.cols-1)LoG_img_a_result.at<double>(x+1,y)=0;
      //if (y!=LoG_img_a_result.rows-1&&x!=0)LoG_img_a_result.at<double>(x-1,y+1)=0;
      //if (y!=LoG_img_a_result.rows-1)LoG_img_a_result.at<double>(x, y+1)=0;
      //if (y!=LoG_img_a_result.rows-1&&x!=LoG_img_a_result.cols-1)LoG_img_a_result.at<double>(x+1,y+1)=0;
    }
  }
//  cv::imshow("LoG-img-a", LoG_img_a);
//  cv::imwrite("../img/LoG/LoG-img-a.png", LoG_img_a);
  cv::imshow("result", LoG_img_a_result);
  cv::imwrite("../img/LoG/LoG-img-a-result.png", LoG_img_a_result);
  
  cv::Mat LoG_img_b_result = cv::Mat::zeros(LoG_rows, LoG_cols, CV_8U);
  {
    //cv::Mat Sobel_filter = cv::Mat::zeros(3, 3, CV_64F);
    std::vector<std::vector<double>> Sobel_filter {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

//    Sobel_filter.at<double>(0,0) =-1;Sobel_filter.at<double>(0,1) = 0;Sobel_filter.at<double>(0,2) = 1;
//    Sobel_filter.at<double>(1,0) =-2;Sobel_filter.at<double>(1,1) = 0;Sobel_filter.at<double>(1,2) = 2;
//    Sobel_filter.at<double>(2,0) =-1;Sobel_filter.at<double>(2,1) = 0;Sobel_filter.at<double>(2,2) = 1;

    cv::Mat tmp_img;
    cv::filter2D(gaussian_img, tmp_img, gaussian_img.type(), Sobel_filter);
    for (auto y = 0;y < LoG_rows;++y) {
      for (auto x = 0;x < LoG_cols;++x) {
        //      double val = LoG_img_a.at<double>(x,y);
        if (tmp_img.at<double>(x,y) > thread_Sobel) {
          LoG_img_b_result.at<unsigned char>(x,y) = 255;
        }
      }
    }
//    Sobel_filter.at<double>(0,0) =-1;Sobel_filter.at<double>(0,1) =-2;Sobel_filter.at<double>(0,2) =-1;
//    Sobel_filter.at<double>(1,0) = 0;Sobel_filter.at<double>(1,1) = 0;Sobel_filter.at<double>(1,2) = 0;
//    Sobel_filter.at<double>(2,0) = 1;Sobel_filter.at<double>(2,1) = 2;Sobel_filter.at<double>(2,2) = 1;
//    cv::Mat tmp2_img;
//    cv::filter2D(gaussian_img, tmp2_img, gaussian_img.type(), Sobel_filter);
  }
  cv::imshow("b-result", LoG_img_b_result);
  cv::waitKey(0);
}
