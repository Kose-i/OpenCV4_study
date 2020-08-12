
#include <opencv2/opencv.hpp>

int main (int argc, char **argv) {

  // (1)画像を読み込む
  if (argc < 2) return -1;
  cv::Mat src_img = cv::imread(argv[1], 0);

  if (src_img.empty()) return -1;

  std::vector<int> hist_data(256, 0);
  for (auto y = 0;y < src_img.rows;++y) {
    for (auto x = 0;x < src_img.cols;++x) {
      ++hist_data[static_cast<int>(src_img.at<unsigned char>(y,x))];
    }
  }

  cv::Mat hist_img;

  cv::imshow("Image", src_img);
  cv::imshow("Histogram", hist_img);
  while(cvWaitKey (0) != 'q');
}
//int main (int argc, char **argv) {
//
//  // (1)画像を読み込む
//  if (argc < 2) return -1;
//  cv::Mat src_img = cv::imread(argv[1], 0);
//
//  if (src_img.empty()) return -1;
//
//  // (2)入力画像のチャンネル数分の画像領域を確保
//  int sch = 1;// = src_img->nChannels;
//  cv::Mat dst_img(src_img.rows, src_img.cols, CV_8UC1);
//  for (auto y = 0;y < src_img.rows;++y) {
//    for (auto x  = 0;x < src_img.cols;++x) {
//      dst_img.at<unsigned char>(y,x) = src_img.at<unsigned char>(y,x);
//    }
//  }
//
//  // (3)ヒストグラム構造体，ヒストグラム画像領域を確保
//  //hist_img = cvCreateImage (cvSize (ch_width * sch, 200), 8, 1);
//  const int ch_width = 256, ch_height = 128;
//  cv::Mat hist_img(cv::Size(ch_width, ch_height), CV_8UC1, cv::Scalar::all(255));
//
//  const int hdims[] = {256};
//  const float hranges[] = { 0, 256 };
//  const float *ranges[] = { hranges };
//  double max_value = 0, min_value = 0;;
//  //CvHistogram *hist;
//  //hist = cvCreateHist (1, &hist_size, CV_HIST_ARRAY, ranges, 1);
//  //cv::Mat hist;
//  cv::Mat hist;
//
//  cv::calcHist(&src_img, 1, 0, cv::Mat(), hist, 1, hdims, ranges, true, false);
//  cv::minMaxLoc(hist, &min_value, &max_value, 0, 0);
//  cv::imshow("hist", hist);
//  //std::cout << hist.rows << ' ' << hist.cols << '\n';
//  for (auto y = 0;y < hist.rows;++y) {
//    std::cout << static_cast<int>(hist.at<unsigned char>(y,0)) << ' ';
//  }
//  std::cout << '\n';
//  //  // (6)ヒストグラムを計算して，スケーリング
//  //  //cvCalcHist (&dst_img[i], hist, 0, NULL);
//  //  //cvGetMinMaxHistValue (hist, 0, &max_value, 0, 0);
//  //  cvScale (hist->bins, hist->bins, ((double) hist_img->height) / max_value, 0);
//  //  // (7)ヒストグラムの描画
//  int hist_size = 256;
//  int bin_w = static_cast<int>((double) ch_width / hist_size);
//  for (int j = 0; j < hist_size; j++)
//    cv::rectangle(hist_img,
//      cv::Point(j * bin_w, hist_img.rows),
//      cv::Point((j + 1) * bin_w, hist_img.rows - static_cast<int>(hist.at<unsigned char>(0, j))),
//      //hist_img.rows - static_cast<int>(cvGetReal1D (hist->bins, j))), cvScalarAll (0), -1, 8, 0);
//      cv::Scalar::all(0), -1, 8, 0);
//
//  // (8)ヒストグラム画像を表示，キーが押されたときに終了
//  cv::imshow("Image", src_img);
//  cv::imshow("Histogram", hist_img);
//  while(cvWaitKey (0) != 'q');
//}
