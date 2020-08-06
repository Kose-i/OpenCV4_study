#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <utility>

using Pair = std::pair<int, int>;

#include "align_img.hpp"
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
  //std::cout << checkbox_pos.size() << '\n'; //42
  //cv::imshow("template-img", template_img);
  //while(cv::waitKey(0)!='q');

  /*パラメータ*/
  constexpr int diff_eps {10};//align?10:1
  std::vector<Pair> questionnair_vec {{2, 0}, {6, 0}, {9, 0}, {7, 1}, {8, 1}, {5, 0}, {5, 0}};
  /*結果を収納するベクタ*/
  std::vector<std::vector<int>> questionnair_ans(questionnair_vec.size());
  for (auto i = 0;i < questionnair_vec.size();++i) {
    std::vector<int> tmp(questionnair_vec[i].first+1, 0);
    questionnair_ans[i] = tmp;
  }


  /*チェック画像の入力*/
  /*N回実行*/
  for (int i = 1;i < 3;++i){
    //const std::string check_file_name {"../dataset/Target2.jpg"};
    std::string check_file_name {"../dataset/Target"};
    check_file_name += std::to_string(i);
    check_file_name += ".jpg";
    cv::Mat check_img = cv::imread(check_file_name, CV_LOAD_IMAGE_GRAYSCALE);
    cv::imshow(check_file_name+"-img", check_img);

    cv::Mat align_check_img = align_img::align(template_img, check_img);
    cv::Mat diff_img = check_img;
    for (auto y = 0;y < diff_img.rows;++y) {
      for (auto x = 0;x < diff_img.cols;++x) {
        int a = static_cast<int>(align_check_img.at<unsigned char>(y,x));
        int b = static_cast<int>(template_img.at<unsigned char>(y,x));
        if (std::abs(a-b) < diff_eps) {
          diff_img.at<unsigned char>(y, x) = 0;
        } else {
          diff_img.at<unsigned char>(y, x) = 255;
        }
      }
    }
    cv::Mat median_diff_img = diff_img;
    medianBlur(diff_img, median_diff_img, 3);//(3,3);

    /*テスト画像とチェック領域からチェックの抽出*/

    /*１アンケート結果の調査*/
    std::vector<std::vector<int>> one_data(questionnair_vec.size());
    std::vector<int> checkbox_inner(checkbox_pos.size(), 0);
    for (auto i = 0;i < questionnair_vec.size();++i) {
      std::vector<int> tmp(questionnair_vec[i].first+1, 0);
      one_data[i] = tmp;
    }
    for (auto i = 0;i < checkbox_pos.size();++i) {
      Box tmp = checkbox_pos[i];
      int ans {};
      for (auto y = tmp.lt.y;y < tmp.rb.y;++y) {
        for (auto x = tmp.lt.x;x < tmp.rb.x;++x) {
          if (diff_img.at<unsigned char>(y,x)==255) ++ans;
        }
      }
      checkbox_inner[i] = ans;
    }
    int now_index {};
    for (auto i = 0;i < questionnair_vec.size();++i) {
      std::vector<int> c_tmp;
      for (auto j = 0;j < questionnair_vec[i].first;++j) {
        if (checkbox_inner[now_index]>(checkbox_pos[i].rb.y - checkbox_pos[i].lt.y)*(checkbox_pos[i].rb.x - checkbox_pos[i].lt.x)/8) {
          c_tmp.push_back(j);
        }
        ++now_index;
      }
      if (c_tmp.size() == 1) {
        ++one_data[i][c_tmp[0]];
      } else if (c_tmp.size() > 1 && questionnair_vec[i].second == 1) {
        for (const auto& e : c_tmp) {
          ++one_data[i][e];
        }
      } else {
        ++one_data[i][one_data[i].size()-1];
      }
    }

    // 集計
    for (auto i = 0;i < questionnair_vec.size();++i) {
      for (auto j = 0;j < questionnair_vec[i].first+1;++j) {
        questionnair_ans[i][j] += one_data[i][j];
      }
    }
  }
  //結果の表示
  for (const auto& e : questionnair_ans) {
    for (const auto& f : e) {
      std::cout << ' ' << f;
    }
    std::cout << '\n';
  }
  while(cv::waitKey(0) != 'q');
}
