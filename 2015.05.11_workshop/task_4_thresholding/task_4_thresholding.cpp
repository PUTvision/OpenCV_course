#include "opencv2/opencv.hpp"

using namespace cv;

#include <iostream>

int main(int, char)
{
	Mat imageFromFile;
	imageFromFile = imread("..\\data\\zaliczenie_monety.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat imageBlurred;
	blur(imageFromFile, imageBlurred, Size(5, 5));

	Mat imageThresholded;
	threshold(imageBlurred, imageThresholded, 100, 255, THRESH_BINARY_INV);

	Mat imageEroded;
	erode(imageThresholded, imageEroded, Mat());

	Mat structingElement = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));

	Mat imageDilated;
	dilate(imageEroded, imageDilated, structingElement);

	namedWindow("Image from file", WINDOW_AUTOSIZE);
	imshow("Image from file", imageDilated);

	waitKey(0);

	return 0;
}