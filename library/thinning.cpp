using namespace cv;
void thinningIte(Mat& img, int pattern){
    Mat del_marker = Mat::ones(img.size(), CV_8UC1);
    int x, y;

    for (y = 1; y < img.rows-1; ++y) {
        for (x = 1; x < img.cols-1; ++x) {
            int v9,v2,v3;
            int v8,v1,v4;
            int v7,v6,v5;
            
            v1=img.data[   y   * img.step +   x   * img.elemSize()];
            v2=img.data[ (y-1) * img.step +   x   * img.elemSize()];
            v3=img.data[ (y-1) * img.step + (x+1) * img.elemSize()];
            v4=img.data[   y   * img.step + (x+1) * img.elemSize()];
            v5=img.data[ (y+1) * img.step + (x+1) * img.elemSize()];
            v6=img.data[ (y+1) * img.step +   x   * img.elemSize()];
            v7=img.data[ (y+1) * img.step + (x-1) * img.elemSize()];
            v8=img.data[   y   * img.step + (x-1) * img.elemSize()];
            v9=img.data[ (y-1) * img.step + (x-1) * img.elemSize()];
            
            int S  = (v2 == 0 && v3 == 1) + (v3 == 0 && v4 == 1) +
            (v4 == 0 && v5 == 1) + (v5 == 0 && v6 == 1) +
            (v6 == 0 && v7 == 1) + (v7 == 0 && v8 == 1) +
            (v8 == 0 && v9 == 1) + (v9 == 0 && v2 == 1);
            
            int N  = v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9;
            
            int m1=0, m2=0;
            
            if(pattern==0) m1 = (v2 * v4 * v6);
            if(pattern==1) m1 = (v2 * v4 * v8);
            
            if(pattern==0) m2 = (v4 * v6 * v8);
            if(pattern==1) m2 = (v2 * v6 * v8);
            
            if (S == 1 && (N >= 2 && N <= 6) && m1 == 0 && m2 == 0)
                del_marker.data[y * del_marker.step + x * del_marker.elemSize()]=0;
        }
    }
    
    img &= del_marker;
}

void thinning(const Mat& src, Mat& dst){
    dst = src.clone();
    dst /= 255;         // 0は0 , 1以上は1に変換される
    
    Mat prev = Mat::zeros(dst.size(), CV_8UC1);
    Mat diff;
    
    do {
        thinningIte(dst, 0);
        thinningIte(dst, 1);
        absdiff(dst, prev, diff);
        dst.copyTo(prev);
    }while (countNonZero(diff) > 0);
    
    dst *= 255;
}

int main(){
    Mat img = imread("thinning.jpg");
    
    Mat img2;
    cvtColor(img, img2, CV_BGR2GRAY);
    threshold(img2, img2, 10, 255, THRESH_BINARY);
    
    thinning(img2, img2);
    
    imshow("src", img);
    imshow("dst", img2);
    waitKey();
    return 0;
}
