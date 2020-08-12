#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <utility>

using Pair = std::pair<int, int>;

#include "get_checkbox.hpp"

bool operator<(Box& l, Box& r) {
  //return (l.lt.y < r.lt.y)?true;//(l.lt.x<r.lt.x);
  if (l.lt.y==r.lt.y) {
    return (l.lt.x<r.lt.x);
  }
  return (l.lt.y < r.lt.y);
}

int main(int argc, char** argv) {

  /*テンプレート画像からチェック領域の抽出*/
  const std::string template_file_name {"../dataset/Template.jpg"};
  cv::Mat template_img = cv::imread(template_file_name, CV_LOAD_IMAGE_GRAYSCALE);
  std::vector<Box> checkbox_pos = get_checkbox_pos(template_img);
  std::sort(checkbox_pos.begin(), checkbox_pos.end());
  cv::Mat template_mask(template_img.rows, template_img.cols, template_img.type());
  for (auto y = 0;y < template_mask.rows;++y) {
    for (auto x = 0;x < template_mask.cols;++x) {
      template_mask.at<unsigned char>(y,x) = 0;
    }
  }
  for (const auto& box : checkbox_pos) {
    for (auto y = box.lt.y;y < box.rb.y;++y) {
      for (auto x = box.lt.x;x < box.rb.x;++x) {
        template_mask.at<unsigned char>(y,x) = 255;
      }
    }
  }
  cv::imshow("template-mask", template_mask);
  cv::imwrite("template-mask.jpg", template_mask);

  const std::string check_file_name {"../dataset/Target1.jpg"};
  cv::Mat check_img = cv::imread(check_file_name, CV_LOAD_IMAGE_GRAYSCALE);
  cv::imshow("target-img", check_img);
  cv::Mat img = cv::Mat::zeros(check_img.rows, check_img.cols, check_img.type());

  for (auto y = 0;y < check_img.rows;++y) {
    for (auto x = 0;x < check_img.cols;++x) {
      if (template_mask.at<unsigned char>(y,x)==255) {
        img.at<unsigned char>(y,x) = 255 - check_img.at<unsigned char>(y,x);
      } else {
        img.at<unsigned char>(y,x) = 0;
      }
    }
  }
  cv::imshow("after_mask", img);
  cv::imwrite("after_mask.jpg", img);
  while(cv::waitKey(0)!='q') ;

  /*パラメータ*/
  //constexpr int diff_eps {10};//align?10:1
  //std::vector<Pair> questionnair_vec {{2, 0}, {6, 0}, {9, 0}, {7, 1}, {8, 1}, {5, 0}, {5, 0}};
  ///*結果を収納するベクタ*/
  //std::vector<std::vector<int>> questionnair_ans(questionnair_vec.size());
  //for (auto i = 0;i < questionnair_vec.size();++i) {
  //  std::vector<int> tmp(questionnair_vec[i].first+1, 0);
  //  questionnair_ans[i] = tmp;
  //}


  /*チェック画像の入力*/
  /*N回実行*/
  //for (int i = 1;i < 3;++i){
  //  //const std::string check_file_name {"../dataset/Target2.jpg"};
  //  std::string check_file_name {"../dataset/Target"};
  //  check_file_name += std::to_string(i);
  //  check_file_name += ".jpg";
  //  cv::Mat check_img = cv::imread(check_file_name, CV_LOAD_IMAGE_GRAYSCALE);
  //  cv::imshow(check_file_name+"-img", check_img);

  //  cv::Mat align_check_img = align_img::align(template_img, check_img);
  //  cv::Mat diff_img = check_img;
  //  for (auto y = 0;y < diff_img.rows;++y) {
  //    for (auto x = 0;x < diff_img.cols;++x) {
  //      int a = static_cast<int>(align_check_img.at<unsigned char>(y,x));
  //      int b = static_cast<int>(template_img.at<unsigned char>(y,x));
  //      if (std::abs(a-b) < diff_eps) {
  //        diff_img.at<unsigned char>(y, x) = 0;
  //      } else {
  //        diff_img.at<unsigned char>(y, x) = 255;
  //      }
  //    }
  //  }
  //  cv::Mat median_diff_img = diff_img;
  //  medianBlur(diff_img, median_diff_img, 3);//(3,3);

  //  /*テスト画像とチェック領域からチェックの抽出*/

  //  /*１アンケート結果の調査*/
  //  std::vector<std::vector<int>> one_data(questionnair_vec.size());
  //  std::vector<int> checkbox_inner(checkbox_pos.size(), 0);
  //  for (auto i = 0;i < questionnair_vec.size();++i) {
  //    std::vector<int> tmp(questionnair_vec[i].first+1, 0);
  //    one_data[i] = tmp;
  //  }
  //  for (auto i = 0;i < checkbox_pos.size();++i) {
  //    Box tmp = checkbox_pos[i];
  //    int ans {};
  //    for (auto y = tmp.lt.y;y < tmp.rb.y;++y) {
  //      for (auto x = tmp.lt.x;x < tmp.rb.x;++x) {
  //        if (diff_img.at<unsigned char>(y,x)==255) ++ans;
  //      }
  //    }
  //    checkbox_inner[i] = ans;
  //  }
  //  int now_index {};
  //  for (auto i = 0;i < questionnair_vec.size();++i) {
  //    std::vector<int> c_tmp;
  //    for (auto j = 0;j < questionnair_vec[i].first;++j) {
  //      if (checkbox_inner[now_index]>(checkbox_pos[i].rb.y - checkbox_pos[i].lt.y)*(checkbox_pos[i].rb.x - checkbox_pos[i].lt.x)/8) {
  //        c_tmp.push_back(j);
  //      }
  //      ++now_index;
  //    }
  //    if (c_tmp.size() == 1) {
  //      ++one_data[i][c_tmp[0]];
  //    } else if (c_tmp.size() > 1 && questionnair_vec[i].second == 1) {
  //      for (const auto& e : c_tmp) {
  //        ++one_data[i][e];
  //      }
  //    } else {
  //      ++one_data[i][one_data[i].size()-1];
  //    }
  //  }

  //  // 集計
  //  for (auto i = 0;i < questionnair_vec.size();++i) {
  //    for (auto j = 0;j < questionnair_vec[i].first+1;++j) {
  //      questionnair_ans[i][j] += one_data[i][j];
  //    }
  //  }
  //}
  ////結果の表示
  //for (const auto& e : questionnair_ans) {
  //  for (const auto& f : e) {
  //    std::cout << ' ' << f;
  //  }
  //  std::cout << '\n';
  //}
  //while(cv::waitKey(0) != 'q');
}

//int main(int argc, char** argv) {
//
//  const std::string template_file_name {"../dataset/Template.jpg"};
//  cv::Mat template_img = cv::imread(template_file_name, CV_LOAD_IMAGE_GRAYSCALE);
//  cv::Mat test(template_img.cols*2+1, template_img.rows*3+2, template_img.type());
//  for (auto y_ratio = 0;y_ratio < 2;++y_ratio) {
//    for (auto x_ratio = 0;x_ratio < 3;++x_ratio) {
//      auto y = 0;
//      for (y = 0;y < template_img.cols;++y) {
//        auto x = 0;
//        for (x = 0;x < template_img.rows;++x) {
//      test.at<unsigned char>(template_img.cols*y_ratio+y_ratio+y, template_img.rows*x_ratio+x_ratio+x) = template_img.at<unsigned char>(y, x);
//        }
//        test.at<unsigned char>(template_img.cols*y_ratio+y_ratio+y, template_img.rows*x_ratio+x_ratio+x) = 0;
//      }
//      for (auto x = 0;x < template_img.rows;++x) {
//        test.at<unsigned char>(template_img.cols*y_ratio+y_ratio+y, template_img.rows*x_ratio+x_ratio+x) = 0;
//      }
//    }
//  }
//  cv::imshow("test", test);
//  cv::imwrite("test.jpg", test);
//  cv::imwrite("test.png", test);
//  while(cv::waitKey(0)!='q') ;
//}
