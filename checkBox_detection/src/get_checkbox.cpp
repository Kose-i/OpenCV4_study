#include <iostream>
#include <string>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "get_checkbox.hpp"

//struct Box {
//  cv::Point lt, rb;
//};
//
bool is_in(const Box& l, const Box& r) {
  if (l.lt.x < r.lt.x && l.lt.y < r.lt.y) {
    if (r.rb.x < l.rb.x && r.rb.y < l.rb.y) {
      return true;
    }
  }
  return false;
}

std::vector<Box> get_checkbox_pos(const cv::Mat& template_img) {
  /*
  ** Input
  ** template_img : gray
  **
  */

  cv::Mat img_contours(template_img.rows, template_img.cols, template_img.type());
  img_contours = template_img;

  cv::threshold(img_contours, img_contours,200,255, cv::THRESH_BINARY);

  std::vector<std::vector<cv::Point>> output_contours;
  cv::findContours(img_contours, output_contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
  img_contours = cv::Scalar::all(0);
  //cv::drawContours(img_contours, output_contours, -1, cv::Scalar::all(255));

  std::vector<cv::Point> approx;

  //cv::Mat dist_img(img_contours.cols, img_contours.rows, img_contours.type());
  //dist_img = cv::Scalar::all(0);
  std::vector<Box> points_candidate;

  for (const auto& curve : output_contours) {
    std::vector<cv::Point> approx;
    cv::approxPolyDP(curve, approx, 1.0, true);
    if (approx.size() == 4) {
      cv::Point lt{approx[0]};
      cv::Point rb{approx[0]};
      //std::cout << "check\n";
      bool is_ok {true};
      for (auto i = 0;i < 3;++i) {
        int len1 = std::max(std::abs(approx[i].y - approx[i+1].y), std::abs(approx[i].x - approx[i+1].x));
        int len2 = std::min(std::abs(approx[i].y - approx[i+1].y), std::abs(approx[i].x - approx[i+1].x));
        lt.x = std::min(lt.x, approx[i+1].x);
        lt.y = std::min(lt.y, approx[i+1].y);
        rb.x = std::max(rb.x, approx[i+1].x);
        rb.y = std::max(rb.y, approx[i+1].y);
        if (2 < len2 || len1 < 2 || 50 < len1) {
          is_ok = false;
          break;
        }
      }
      if (is_ok == false) continue;
      Box box_tmp;
      box_tmp.lt = lt;
      box_tmp.rb = rb;
      points_candidate.push_back(box_tmp);

      //for (const auto& p : approx) {
      //  dist_img.at<unsigned char>(p.y, p.x) = 255;
      //  std::cout << p.y << ' ' << p.x << ' ';
      //}
      //std::cout << '\n';

      //cv::rectangle(img, lt, rb, cv::Scalar(100, 100, 100));//draw rectangle

    }
  }

  std::vector<Box> box_define;
  //std::cout << points_candidate.size() << '\n';
  //for (const auto& e : points_candidate) {
  //    cv::rectangle(dist_img, e.lt, e.rb, cv::Scalar(255));//draw rectangle
  //}


  for (const auto& e : points_candidate) {
    for (const auto& f : points_candidate) {
      if (is_in(e, f)) { //e > f
        box_define.push_back(e);
        break;
      }
    }
  }
  //std::cout << box_define.size() << '\n';
  //for (const auto& box : box_define) {
  //    cv::rectangle(dist_img, box.lt, box.rb, cv::Scalar(255));//draw rectangle
  //}

  //cv::imshow("img", img);
  //cv::imshow("img-contours", img_contours);
  //cv::imshow("dist", dist_img);

  //while(cv::waitKey(0)!='q');
  return box_define;
}
//int main(int argc, char** argv) {
//
//  const std::string file_name {"../dataset/Template.jpg"};
//  cv::Mat img = cv::imread(file_name); // color
//  cv::Mat img_contours = cv::imread(file_name, CV_LOAD_IMAGE_GRAYSCALE);
//
//  cv::threshold(img_contours, img_contours,200,255, cv::THRESH_BINARY);
//
//  std::vector<std::vector<cv::Point>> output_contours;
//  cv::findContours(img_contours, output_contours, cv::noArray(), cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
//  img_contours = cv::Scalar::all(0);
//  cv::drawContours(img_contours, output_contours, -1, cv::Scalar::all(255));
//
//  std::vector<cv::Point> approx;
//
//  cv::Mat dist_img(img_contours.cols, img_contours.rows, img_contours.type());
//  dist_img = cv::Scalar::all(0);
//  std::vector<Box> points_candidate;
//
//  for (const auto& curve : output_contours) {
//    std::vector<cv::Point> approx;
//    cv::approxPolyDP(curve, approx, 1.0, true);
//    if (approx.size() == 4) {
//      cv::Point lt{approx[0]};
//      cv::Point rb{approx[0]};
//      //std::cout << "check\n";
//      bool is_ok {true};
//      for (auto i = 0;i < 3;++i) {
//        int len1 = std::max(std::abs(approx[i].y - approx[i+1].y), std::abs(approx[i].x - approx[i+1].x));
//        int len2 = std::min(std::abs(approx[i].y - approx[i+1].y), std::abs(approx[i].x - approx[i+1].x));
//        lt.x = std::min(lt.x, approx[i+1].x);
//        lt.y = std::min(lt.y, approx[i+1].y);
//        rb.x = std::max(rb.x, approx[i+1].x);
//        rb.y = std::max(rb.y, approx[i+1].y);
//        if (2 < len2 || len1 < 2 || 50 < len1) {
//          is_ok = false;
//          break;
//        }
//      }
//      if (is_ok == false) continue;
//      Box box_tmp;
//      box_tmp.lt = lt;
//      box_tmp.rb = rb;
//      points_candidate.push_back(box_tmp);
//
//      //for (const auto& p : approx) {
//      //  dist_img.at<unsigned char>(p.y, p.x) = 255;
//      //  std::cout << p.y << ' ' << p.x << ' ';
//      //}
//      //std::cout << '\n';
//
//      //cv::rectangle(img, lt, rb, cv::Scalar(100, 100, 100));//draw rectangle
//
//    }
//  }
//
//  std::vector<Box> box_define;
//  //std::cout << points_candidate.size() << '\n';
//  //for (const auto& e : points_candidate) {
//  //    cv::rectangle(dist_img, e.lt, e.rb, cv::Scalar(255));//draw rectangle
//  //}
//
//
//  for (const auto& e : points_candidate) {
//    for (const auto& f : points_candidate) {
//      if (is_in(e, f)) { //e > f
//        box_define.push_back(e);
//        break;
//      }
//    }
//  }
//  std::cout << box_define.size() << '\n';
//  for (const auto& box : box_define) {
//      cv::rectangle(dist_img, box.lt, box.rb, cv::Scalar(255));//draw rectangle
//  }
//
//  cv::imshow("img", img);
//  cv::imshow("img-contours", img_contours);
//  cv::imshow("dist", dist_img);
//
//  while(cv::waitKey(0)!='q');
//}
