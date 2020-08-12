#include "align_img.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

cv::Mat align_img::align(const cv::Mat& template_img, const cv::Mat& img) {

  cv::Mat output_img(img.rows, img.cols, img.type());
  cv::medianBlur(img, output_img, 7);//(3,3);
  cv::adaptiveThreshold(output_img, output_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 71, 15);
  //cv::threshold(output_img, output_img,100,255, cv::THRESH_BINARY);
  for (auto y=0;y < output_img.rows;++y) {
    for (auto x=0;x < output_img.cols;++x) {
      output_img.at<unsigned char>(y,x) = 255 - output_img.at<unsigned char>(y,x);
    }
  }



  //cv::resize(output_img, output_img, cv::Size(template_img.rows, template_img.cols));
  std::vector<cv::Point2d> point_edge(4);
  bool is_first {true};
  for (auto y=0;y < output_img.rows;++y) {
    for (auto x=0;x < output_img.cols;++x) {
      if (is_first && output_img.at<unsigned char>(y,x)==255) {
        for (auto& e : point_edge) {
          e.y = y;  e.x = x;
        }
        is_first = false;
      } else if (output_img.at<unsigned char>(y,x)==255&&\
      output_img.at<unsigned char>(y-1,x-1)==255&&\
      output_img.at<unsigned char>(y-1,x)==255&&\
      output_img.at<unsigned char>(y-1,x+1)==255&&\
      output_img.at<unsigned char>(y,x-1)==255&&\
      output_img.at<unsigned char>(y,x+1)==255&&\
      output_img.at<unsigned char>(y+1,x-1)==255&&\
      output_img.at<unsigned char>(y+1,x)==255&&\
      output_img.at<unsigned char>(y+1,x+1)==255) {
        if (point_edge[0].y > y) {//top
          point_edge[0].y = y; point_edge[0].x = x;
        }
        if (point_edge[1].x < x) {//right
          point_edge[1].y = y; point_edge[1].x = x;
        }
        if (point_edge[2].y < y) {//bottom
          point_edge[2].y = y; point_edge[2].x = x;
        }
        if (point_edge[3].x > x) {//left
          point_edge[3].y = y; point_edge[3].x = x;
        }
      }
    }
  }
  //for (const auto& e : point_edge) {
  //  std::cout << e.y << ' ' << e.x << '\n';
  //}
  //cv::circle(output_img, point_edge[0], 30, cv::Scalar(255));
  //cv::circle(output_img, point_edge[1], 30, cv::Scalar(255));
  //cv::circle(output_img, point_edge[2], 30, cv::Scalar(255));
  //cv::circle(output_img, point_edge[3], 30, cv::Scalar(255));
  //cv::imshow("output_img", output_img);
  //cv::waitKey(0);
  //return output_img;
  for (auto y=0;y < output_img.rows;++y) {
    for (auto x=0;x < output_img.cols;++x) {
      output_img.at<unsigned char>(y,x) = 255 - output_img.at<unsigned char>(y,x);
    }
  }
  cv::medianBlur(output_img, output_img, 7);//(3,3);
  //return output_img;

  //cv::Mat img_contours(img.rows, img.cols, img.type());
  //for (auto y = 0;y < img.rows;++y) {
  //  for (auto x = 0;x < img.cols;++x) {
  //    img_contours.at<unsigned char>(y,x) = img.at<unsigned char>(y,x);
  //  }
  //}
  //cv::resize(img_contours, img_contours, cv::Size(template_img.rows, template_img.cols));

  ////cv::threshold(img_contours, img_contours,100,255, cv::THRESH_BINARY);
  ////return img_contours;

  //int max_len {};

  //std::vector<std::vector<cv::Point>> output_contours;
  //cv::findContours(img_contours, output_contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
  //std::cout << output_contours.size() << '\n';
  //for (const auto& curve : output_contours) {
  //  std::vector<cv::Point> approx;
  //  cv::approxPolyDP(curve, approx, 1.0, true);
  //  if (approx.size() == 4) {
  //    if (max_len < std::abs(approx[0].y-approx[1].y)+std::abs(approx[0].x-approx[1].x)) {
  //      point_edge[0] = approx[0];
  //      point_edge[1] = approx[1];
  //      point_edge[2] = approx[2];
  //      point_edge[3] = approx[3];
  //      max_len = std::abs(approx[0].y-approx[1].y)+std::abs(approx[0].x-approx[1].x);
  //    }
  //  }
  //}
  ////return img_contours;

  std::vector<cv::Point2f> dst_ptr(4);
  dst_ptr[0].y = 0; dst_ptr[0].x = 0;
  dst_ptr[1].y = 0; dst_ptr[1].x = template_img.cols;
  dst_ptr[2].y = template_img.rows; dst_ptr[2].x = template_img.cols;
  dst_ptr[3].y = template_img.rows; dst_ptr[3].x = 0;

  std::vector<cv::Point2f> src_ptr(4);//optimizer
  unsigned long long min_error {0};

  //{
  //  std::cout << "top "<<point_edge[0].y<<' '<<point_edge[0].x<<'\n';
  //  std::cout << "right "<<point_edge[1].y<<' '<<point_edge[1].x<<'\n';
  //  std::cout << "bottom "<<point_edge[2].y<<' '<<point_edge[2].x<<'\n';
  //  std::cout << "left "<<point_edge[3].y<<' '<<point_edge[3].x<<'\n';
  //}
  // 大まかな位置調整
  for (auto i = 0;i < 4;++i) {
    std::vector<cv::Point2f> src_ptr_tmp(4);
    src_ptr_tmp[0] = point_edge[i%4];
    src_ptr_tmp[1] = point_edge[(i+1)%4];
    src_ptr_tmp[2] = point_edge[(i+2)%4];
    src_ptr_tmp[3] = point_edge[(i+3)%4];
    //cv::Mat affine_mask = cv::getAffineTransform(src_ptr_tmp, dst_ptr);
    cv::Mat persepective_mask = cv::getPerspectiveTransform(src_ptr_tmp, dst_ptr);
        cv::Mat dst(img.rows, img.cols, img.type());
        //cv::warpAffine(img, dst, affine_mask, img.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
        //cv::warpPerspective(img, dst, persepective_mask, img.size(), cv::WARP_INVERSE_MAP|cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar::all(0));
        cv::warpPerspective(img, dst, persepective_mask, img.size(), cv::INTER_LINEAR|cv::WARP_FILL_OUTLIERS, cv::BORDER_CONSTANT, cv::Scalar::all(0));
        //cv::Mat dist_img(template_img.rows, template_img.cols, template_img.type(), cv::Scalar::all(0));
        unsigned long long tmp {};
        for (auto y = 0;y < template_img.rows;++y) {
          for (auto x = 0;x < template_img.cols;++x) {
            tmp += (static_cast<int>(dst.at<unsigned char>(y,x))-static_cast<int>(template_img.at<unsigned char>(y,x)))*(static_cast<int>(dst.at<unsigned char>(y,x))-static_cast<int>(template_img.at<unsigned char>(y,x)));
          }
        }
        //TODO Delete
        cv::Mat tmp_img(template_img.rows, template_img.cols, template_img.type());
        for (auto y = 0;y < template_img.rows;++y) {
          for (auto x = 0;x < template_img.cols;++x) {
            tmp_img.at<unsigned char>(y,x) = dst.at<unsigned char>(y,x);
          }
        }
        //cv::imshow("tmp-img", tmp_img);
        //while(cv::waitKey(0)!='q') ;
        if (min_error==0) {
          min_error = tmp;
          src_ptr = src_ptr_tmp;
        } else if (min_error > tmp) {
          min_error = tmp;
          src_ptr = src_ptr_tmp;
        }
        //std::cout << tmp << ' ' << min_error << '\n';
  }
  // 微小位置調整
  //for (auto first=0;first<1;++first) {
  //  for (auto i = 0;i < 3;++i) {//src_ptr.size() == 3
  //    int change_diff = 20;
  //    for (auto j = 0;j < 4;++j) { // <-x->  <-y->
  //      if (j==0){
  //        src_ptr[i].x -= change_diff;
  //      } else if (j==1) {
  //        src_ptr[i].y -= change_diff;
  //      } else if (j==2) {
  //        src_ptr[i].x += change_diff;
  //      } else if (j==3) {
  //        src_ptr[i].y += change_diff;
  //      }
  //      cv::Mat affine_mask = cv::getAffineTransform(src_ptr, dst_ptr);
  //      cv::Mat dst(img.rows, img.cols, img.type());

  //      cv::threshold(dst, dst,100,255, cv::THRESH_BINARY);

  //      cv::warpAffine(img, dst, affine_mask, img.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
  //      unsigned long long tmp {};
  //      for (auto y = 0;y < template_img.rows;++y) {
  //        for (auto x = 0;x < template_img.cols;++x) {
  //          tmp += (static_cast<int>(dst.at<unsigned char>(y,x))-static_cast<int>(template_img.at<unsigned char>(y,x)))*(static_cast<int>(dst.at<unsigned char>(y,x))-static_cast<int>(template_img.at<unsigned char>(y,x)));
  //        }
  //      }
  //      if (min_error==0) {
  //        min_error = tmp;
  //      } else if (min_error > tmp) {
  //        std::cout << change_diff << " change ";
  //        std::cout << j << " index\n";
  //        min_error = tmp;
  //        --j;
  //      } else {
  //        //もとに戻す
  //        if (j==0){
  //          src_ptr[i].x += change_diff;
  //        } else if (j==1) {
  //          src_ptr[i].y += change_diff;
  //        } else if (j==2) {
  //          src_ptr[i].x -= change_diff;
  //        } else if (j==3) {
  //          src_ptr[i].y -= change_diff;
  //        }
  //        change_diff /= 2;
  //        if (change_diff < 2) continue;
  //        --j;
  //      }
  //    }// for j
  //  }// for i
  //}

  //cv::Mat affine_mask = cv::getAffineTransform(src_ptr, dst_ptr);
  cv::Mat persepective_mask = cv::getPerspectiveTransform(src_ptr, dst_ptr);
  cv::Mat dst;
  //cv::warpAffine(img, dst, affine_mask, output_img.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
  cv::warpPerspective(img, dst, persepective_mask, img.size(), cv::INTER_LINEAR|cv::WARP_FILL_OUTLIERS, cv::BORDER_CONSTANT, cv::Scalar::all(0));
  cv::Mat dist_img(template_img.rows, template_img.cols, template_img.type(), cv::Scalar::all(0));
  int tmp {};
  for (auto y = 0;y < template_img.rows;++y) {
    for (auto x = 0;x < template_img.cols;++x) {
      dist_img.at<unsigned char>(y,x) = dst.at<unsigned char>(y,x);
    }
  }
  //cv::threshold(dist_img, dist_img,100,255, cv::THRESH_BINARY);
  //cv::adaptiveThreshold(dist_img, dist_img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 71, 15);
  
  return dist_img;
}
