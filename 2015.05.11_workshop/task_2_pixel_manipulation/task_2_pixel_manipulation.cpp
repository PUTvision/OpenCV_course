#include "opencv2/opencv.hpp"

// Ceemple OpenCV 3.0

using namespace cv;

#include <iostream>

static void drawRectangle(Mat& image, int upperLeftX, int upperLeftY, int width, int height)
{
	for (int i = upperLeftY; i < upperLeftY + height; ++i)
	{
		for (int j = upperLeftX; j < upperLeftX + width; ++j)
		{
			image.at<uchar>(i, j) = 255;
		}
	}
}

int main(int, char)
{
	Mat imageFromFile;

	imageFromFile = imread("..\\data\\dog_wizard.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	std::cout << (int)imageFromFile.at<uchar>(100, 100) << std::endl;

	const int widthSize = 15;
	const int heightSize = 15;

	drawRectangle(imageFromFile, 650, 490, widthSize, heightSize);
	drawRectangle(imageFromFile, 700, 530, widthSize, heightSize);

	namedWindow("Modified image", WINDOW_AUTOSIZE);
	imshow("Modified image", imageFromFile);

	waitKey(0);

	return 0;
}