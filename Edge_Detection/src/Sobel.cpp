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
  cv::Mat src_img = cv::imread(src_img_name);
  if (src_img.empty()) {
    std::cerr << src_img_name << " is not found\n";
    return -1;
  }
  cv::imshow("src-img", src_img); 
  cv::imwrite("../img/Sobel/src-img.png", src_img);
  cv::waitKey(0);

  cv::Mat dst_img = cv::Mat::zeros(src_img.rows, src_img.cols, CV_64F);
  {
    std::vector<cv::Mat> filters = make_sobel_filters();
    //cv::Mat tmp_img = cv::Mat::zeros(src_img.rows, src_img.cols, CV_64F);
    cv::filter2D(src_img, dst_img, src_img.type(), filters[0]);
    //for (const auto& filter : filters) {
    //  std::cout << filter << '\n';
    //  cv::Mat tmp_img = cv::Mat::zeros(src_img.rows, src_img.cols, CV_64F);
    //  cv::filter2D(src_img, tmp_img, src_img.type(), filter);
    //  //    cv::Mat dst_img1 = senkei(tmp_img);
    //  for (auto y = 0;y < dst_img.rows; ++y) {
    //    for (auto x = 0;x < dst_img.cols; ++x) {
    //      dst_img.at<double>(x,y) = std::max(dst_img.at<double>(x,y), std::abs(tmp_img.at<double>(x,y)));
    //    }
    //  }
    //  cv::imshow("check", tmp_img);
    //  cv::imshow("dst-img", dst_img);
    //  cv::waitKey(0);
    //}
  }
  cv::Mat dst_thinning_img = senkei(dst_img);
  //for (int y = 0;y < dst_thinning_img.rows;++y) {
  //  for (int x = 0;x < dst_thinning_img.cols;++x) {
  //    if (dst_thinning_img.at<unsigned char>(x,y) <= 1) {
  //      dst_thinning_img.at<unsigned char>(x,y) = 255;
  //    } else {
  //      dst_thinning_img.at<unsigned char>(x,y) = 0;
  //    }
  //  }
  //}
  cv::threshold(dst_img, dst_thinning_img, 50, 255, cv::THRESH_BINARY);
  cv::imshow("thinning_img", dst_thinning_img);
  cv::waitKey(0);

  cv::Mat dst_thinning_img_result = dst_thinning_img;
  thinning(dst_thinning_img, dst_thinning_img);

  cv::imshow("thinning_result", dst_thinning_img);
  cv::waitKey(0);

  //cv::imshow("dst-img", dst_img);
  //cv::imwrite("../img/Sobel/dst-img.png", dst_img);
  //cv::waitKey(0);
}
