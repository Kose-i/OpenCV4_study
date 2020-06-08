

const string trackbarName {""};
const string windowName {""};
int val {};
int max_val {};

int cv::createTrackbar(trackbarName, windowName, &val, max_val);

void track_bar_callback(int pos);
int cv::createTrackbar(trackbarName, windowName, &val, max_val, track_bar_callback);
