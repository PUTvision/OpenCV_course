#include "opencv2/opencv.hpp"

using namespace cv;

#include <iostream>

static void decodeImage(Mat& image)
{
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			// test if the last bit is set
			if (image.at<uchar>(i, j) & (1 << 0))
			{
				image.at<uchar>(i, j) = 255;
			}
			else
			{
				image.at<uchar>(i, j) = 0;
			}
		}
	}
}

int main(int, char)
{
	Mat imageFromFile;

	imageFromFile = imread("..\\data\\zaliczenie_napis.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	decodeImage(imageFromFile);

	namedWindow("Decoded image", WINDOW_AUTOSIZE);
	imshow("Decoded image", imageFromFile);

	waitKey(0);

	return 0;
}