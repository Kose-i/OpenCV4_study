
constexpr int epsilon {1};

cv::Mat senkei(cv::Mat img) {
  double Min, Max;
  cv::minMaxLoc(img, &Min, &Max);
  img -= Min;
  img.convertTo(dist_img, CV_8U, 255.0/(Max-Min+epsilon));
  return dist_img;
}
