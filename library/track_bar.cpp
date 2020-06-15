#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

void on_low_h_threshold(int,void*);
void on_high_h_threshold(int,void*);
void on_low_s_threshold(int,void*);
void on_high_s_threshold(int,void*);
void on_low_v_threshold(int,void*);
void on_high_v_threshold(int,void*);
int low_h=50,low_s=50,low_v=50;
int high_h=90,high_s=127,high_v=127;
//#endif

int main(int argc, char *argv[])
{
  cv::VideoCapture cap(0);
  namedWindow("Object Detection",WINDOW_NORMAL);
  if(!cap.isOpened()){
    return -1;
  }
  //h180s255v255
  cv::createTrackbar("Low H","Object Detection",&low_h,180,on_low_h_threshold);
  createTrackbar("High H","Object Detection",&high_h,180,on_high_h_threshold);
  createTrackbar("Low S","Object Detection",&low_s,255,on_low_s_threshold); 
  createTrackbar("High S","Object Detection",&high_s,255,on_high_s_threshold); 
  createTrackbar("Low V","Object Detection",&low_v,255,on_low_v_threshold); 
  createTrackbar("High V","Object Detection",&high_v,255,on_high_v_threshold); 
  while(1){
    cv::Mat hsv, mask;                  // 画像オブジェクトの宣言

    cv::Mat row_img;
    cap>>row_img;
    cv::cvtColor(row_img, hsv, CV_BGR2HSV);  // 画像をRGBからHSVに変換
    cv::inRange(hsv, cv::Scalar(low_h,low_s, low_v), cv::Scalar(high_h,high_s,high_v), mask); // 色検出でマスク画像の作成
    //#if 0

    cv::Moments mu = cv::moments(mask,false);
    cv::Point2f mc = cv::Point2f(mu.m10/mu.m00 ,mu.m01/mu.m00);
    cv::circle(mask,mc,4,cv::Scalar(100),2,3);
    //circle(img,center,radius,color,thickness,linetype)
    //#endif
    cv::imshow("mask", mask);
    int key = cv::waitKey(1);
    if(key==113){
      break;
    }
#if 0
    else if(key==115){
      cv::imwrite("img2.png",mask);
    }
#endif
  }
  return(0);
}

void on_low_h_threshold(int,void*)
{
  low_h = min(high_h-1,low_h);
  setTrackbarPos("Low H","Object Detection",low_h);
}

void on_high_h_threshold(int,void*)
{
  high_h = max(high_h,low_h+1);
  setTrackbarPos("High H","Object Detection",high_h);
}
void on_low_s_threshold(int,void*)
{
  low_s = min(high_s-1,low_s);
  setTrackbarPos("Low S","Object Detection",low_s);
}
void on_high_s_threshold(int,void*)
{
  high_s = max(high_s,low_s+1);
  setTrackbarPos("High S","Object Detection",high_s);
}
void on_low_v_threshold(int,void*)
{
  low_v = min(high_v-1,low_v);
  setTrackbarPos("Low V","Object Detection",low_v);
}
void on_high_v_threshold(int,void*)
{
  high_v = max(high_v,low_v+1);
  setTrackbarPos("High V","Object Detection",high_v);
}
