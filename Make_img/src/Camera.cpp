#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Videocapture cap(0);

  while (true) {
    cap >> img;
    if (waitKey(1)=='q') break;
  }
}
