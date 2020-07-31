#
#include <vector>

#include <opencv2/opencv.hpp>

struct Point {
  int x, y;
};

namespace template_match{

  constexpr int nearlity_threathold {100};
  constexpr int nearlity_eps {100};

  int nearlity(const cv::Mat& board, const cv::Mat& mask, const int& start_y, const int& start_x) {
    int ans {};
    for (auto y = 0;y < mask.cols;++y) {
      for (auto x = 0;x < mask.rows;++x) {
        if (board.at<unsigned char>(y,x) - mask.at<unsigned char>(y,x) < nearlity_eps) {
          ans += 0;
        } else {
          ans += board.at<unsigned char>(y, x) - mask.at<unsigned char>(y,x);
        }
      }
    }
    return mask.rows*mask.cols - ans;
  }

  std::vector<Point> make_vector(const cv::Mat& board, const cv::Mat& mask) {
    std::vector<Point> ans;
    for (auto y = 0;y < board.cols - mask.cols;++y) {
      for (auto x = 0;x < board.rows - mask.rows;++x) {
        if (template_match::nearlity(board, mask, y, x) > nearlity_threathold) {
          Point t; t.x = x; t.y = y;
          ans.push_back(t);
        }
      }
    }
    return ans;
  }
}
