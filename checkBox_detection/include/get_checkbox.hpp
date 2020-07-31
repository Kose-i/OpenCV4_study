#ifndef get_checkbox_hpp
#define get_checkbox_hpp

struct Box {
  cv::Point lt, rb;
};

bool is_in(const Box& l, const Box& r);

std::vector<Box> get_checkbox_pos(const cv::Mat& template_img);

#endif
