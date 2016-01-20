#include "opencv2/opencv.hpp"

// Ceemple OpenCV 3.0

using namespace cv;

static void encodePatternInLastBitOfImage(Mat& imageToEnocdeIn, const Mat& pattern)
{
	for (int i = 0; i < imageToEnocdeIn.rows; ++i)
	{
		for (int j = 0; j < imageToEnocdeIn.cols; ++j)
		{
			if (pattern.at<uchar>(i, j) != 255)
			{
				imageToEnocdeIn.at<uchar>(i, j) = (imageToEnocdeIn.at<uchar>(i, j) | (1<<0));
			}
			else
			{
				imageToEnocdeIn.at<uchar>(i, j) = (imageToEnocdeIn.at<uchar>(i, j) & ~(1 << 0));
			}
		}
	}
}

int main(int, char)
{
	Mat imageToEnocdeIn = imread("..\\data\\zaliczenie_napis.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat imagePatternToEncode = imread("..\\data\\glowa-01_1024x768.png", CV_LOAD_IMAGE_GRAYSCALE);

	encodePatternInLastBitOfImage(imageToEnocdeIn, imagePatternToEncode);

	imwrite("..\\data\\dog_wizard.png", imageToEnocdeIn);

	return 0;
}