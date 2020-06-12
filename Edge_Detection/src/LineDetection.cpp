#include <opencv2/opencv.hpp>
#include <string>

std::string src_img_name {"../img/lenna.jpg"};

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

constexpr int epsilon {1};

cv::Mat senkei(cv::Mat img) {
  cv::Mat dist_img;
  double Min, Max;
  cv::minMaxLoc(img, &Min, &Max);
  img -= Min;
  std::cout << "Min:" << Min << ' ' << Max << '\n';
  img.convertTo(dist_img, CV_8U, 255.0/(Max-Min+epsilon));
  return dist_img;
}

using namespace cv;
void thinningIte(Mat& img, int pattern){
    Mat del_marker = Mat::ones(img.size(), CV_8UC1);
    int x, y;

    for (y = 1; y < img.rows-1; ++y) {
        for (x = 1; x < img.cols-1; ++x) {
            int v9,v2,v3;
            int v8,v1,v4;
            int v7,v6,v5;
            
            v1=img.data[   y   * img.step +   x   * img.elemSize()];
            v2=img.data[ (y-1) * img.step +   x   * img.elemSize()];
            v3=img.data[ (y-1) * img.step + (x+1) * img.elemSize()];
            v4=img.data[   y   * img.step + (x+1) * img.elemSize()];
            v5=img.data[ (y+1) * img.step + (x+1) * img.elemSize()];
            v6=img.data[ (y+1) * img.step +   x   * img.elemSize()];
            v7=img.data[ (y+1) * img.step + (x-1) * img.elemSize()];
            v8=img.data[   y   * img.step + (x-1) * img.elemSize()];
            v9=img.data[ (y-1) * img.step + (x-1) * img.elemSize()];
            
            int S  = (v2 == 0 && v3 == 1) + (v3 == 0 && v4 == 1) +
            (v4 == 0 && v5 == 1) + (v5 == 0 && v6 == 1) +
            (v6 == 0 && v7 == 1) + (v7 == 0 && v8 == 1) +
            (v8 == 0 && v9 == 1) + (v9 == 0 && v2 == 1);
            
            int N  = v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9;
            
            int m1=0, m2=0;
            
            if(pattern==0) m1 = (v2 * v4 * v6);
            if(pattern==1) m1 = (v2 * v4 * v8);
            
            if(pattern==0) m2 = (v4 * v6 * v8);
            if(pattern==1) m2 = (v2 * v6 * v8);
            
            if (S == 1 && (N >= 2 && N <= 6) && m1 == 0 && m2 == 0)
                del_marker.data[y * del_marker.step + x * del_marker.elemSize()]=0;
        }
    }
    
    img &= del_marker;
}

void thinning(const Mat& src, Mat& dst){
    dst = src.clone();
    dst /= 255;         // 0は0 , 1以上は1に変換される
    
    Mat prev = Mat::zeros(dst.size(), CV_8UC1);
    Mat diff;
    
    do {
        thinningIte(dst, 0);
        thinningIte(dst, 1);
        absdiff(dst, prev, diff);
        dst.copyTo(prev);
    }while (countNonZero(diff) > 0);
    
    dst *= 255;
}

int main(int argc, char** argv) {
  cv::Mat src_img = cv::imread(src_img_name, cv::IMREAD_GRAYSCALE);
  if (src_img.empty()) {
    std::cerr << src_img_name << " is not found\n";
    return -1;
  }
  std::vector<cv::Mat> Sobel_filters = make_sobel_filters();

  cv::Mat dst_img = cv::Mat::zeros(src_img.cols, src_img.rows, CV_8U);
  cv::Mat n_img = cv::Mat::zeros(src_img.cols, src_img.rows, CV_8U);
  int n_index {};
  for (const auto& filter : Sobel_filters) {
    cv::Mat tmp_img = cv::Mat::zeros(src_img.cols, src_img.rows, CV_8U);
    cv::filter2D(src_img, tmp_img, src_img.type(), filter);
    for (auto y = 0;y < src_img.cols;++y) {
      for (auto x = 0;x < src_img.rows;++x) {
        dst_img.at<unsigned char>(x,y) = std::max(dst_img.at<unsigned char>(x,y), tmp_img.at<unsigned char>(x,y));
        if (dst_img.at<unsigned char>(x,y) < tmp_img.at<unsigned char>(x,y)) {
          dst_img.at<unsigned char>(x,y) = tmp_img.at<unsigned char>(x,y);
          n_img.at<unsigned char>(x,y) = n_index;
        }
      }
    }
    ++n_index;
    //cv::imshow("tmp-img", tmp_img);
    //cv::imshow("dst-img", dst_img); 
    //cv::waitKey(0);
  }
  cv::imshow("src-img", src_img); 
  cv::imwrite("../img/LineDetection/src-img.png", src_img);
  cv::waitKey(0);
  cv::imshow("dst-img", dst_img); 
  cv::imwrite("../img/LineDetection/dst-img1.png", dst_img);
  cv::waitKey(0);
  cv::Mat a_img = cv::Mat::zeros(src_img.cols, src_img.rows, CV_8U);
  thinning(dst_img, a_img);
  cv::imshow("a_img", a_img);
  cv::waitKey(0);
  cv::imwrite("../img/LineDetection/dst-img2.png", a_img);
  cv::waitKey(0);

  //n_img
  cv::Mat Line_img = cv::Mat::zeros(src_img.cols, src_img.rows, CV_8U);
  unsigned char thread_deriv {150};
  for (auto y = 0;y < src_img.cols;++y) {
    for (auto x = 0;x < src_img.rows;++x) {
      if (a_img.at<unsigned char>(x,y)==255 && dst_img.at<unsigned char>(x,y) > thread_deriv) {
        Line_img.at<unsigned char>(x,y) = 255;
      }
    }
  }

//  while (true) {
//    unsigned char max_val {0};
//    unsigned char max_p_x {0};
//    unsigned char max_p_y {0};
//    for (auto y = 0;y < src_img.cols;++y) {
//      for (auto x = 0;x < src_img.rows;++x) {
//        if (max_val < dst_img.at<unsigned char>(x,y) && Line_img.at<unsigned char>(x,y)==0) {
//          max_val = dst_img.at<unsigned char>(x,y);
//          max_p_x = x;
//          max_p_y = y;
//        }
//      }
//    }
//    if (max_val < thread_deriv) {
//      break;
//    }
//  }
  cv::imshow("Line-edge", Line_img);
  cv::imwrite("../img/LineDetection/dst_img3.png", Line_img);
  cv::waitKey(0);
}
