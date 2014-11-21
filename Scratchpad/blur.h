#ifndef __BLUR_H_
#define __BLUR_H_

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)

using namespace cv;

void BlurSuperFastCorreceted2(Mat& imgSource, Mat& imgResult);
void BlurSuperFastCorreceted(Mat& imgSource, Mat& imgResult);
void BlurSuperFast(Mat& imgSource, Mat& imgResult);
void BlurFast( Mat& src,  Mat& dst);
void blurWiktor(Mat& imgSource, Mat& imgResult);
void blurTwoPass(Mat& imgSource, Mat& imgResult);
void blur(Mat& imgSource, Mat& imgResult);

#endif