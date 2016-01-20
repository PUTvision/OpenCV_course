#include "opencv2/opencv.hpp"

// Ceemple OpenCV 3.0

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

	const int structingElementSize = 3;
	Mat structingElement = getStructuringElement(
		MORPH_RECT,
		Size(2 * structingElementSize + 1, 2 * structingElementSize + 1),
		Point(structingElementSize, structingElementSize)
	);

	Mat imageDilated;
	dilate(imageEroded, imageDilated, structingElement);

	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	Mat imgForContoursProcessing = imageDilated.clone();
	findContours(imgForContoursProcessing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	// BGR
	Scalar color_5zl(255, 0, 0);
	Scalar color_1gr(0, 0, 255);
	int numberOf5zl = 0;
	int numberOf1gr = 0;

	Mat imgWithContours = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
	for (int i = 0; i<contours.size(); ++i)
	{
		double contourSize = contourArea(contours[i]);
		if (contourSize > 7000)
		{
			drawContours(imgWithContours, contours, i, color_5zl, 1, 8, hierarchy);
			numberOf5zl++;
		}
		else if (contourSize > 3000)
		{
			drawContours(imgWithContours, contours, i, color_1gr, 1, 8, hierarchy);
			numberOf1gr++;
		}
	}

	std::cout << "Number of 5zl coins: " << numberOf5zl << std::endl;
	std::cout << "Number of 1gr coins: " << numberOf1gr << std::endl;

	namedWindow("Image with appropriate contours", WINDOW_AUTOSIZE);
	imshow("Image with appropriate contours", imgWithContours);
	
	waitKey(0);

	return 0;
}