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

	// http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html#find-contours
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;

	Mat imgForContoursProcessing = imageDilated.clone();
	findContours(imgForContoursProcessing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	/// Get the moments
	std::vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	std::vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f((float)(mu[i].m10 / mu[i].m00), (float)(mu[i].m01 / mu[i].m00));
	}

	RNG rng(12345);

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	Mat imgWithContours = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
	for (int i = 0; i<contours.size(); ++i)
	{
		// BGR
		Scalar randomColor(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

		printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
		drawContours(imgWithContours, contours, i, randomColor, 1, 8, hierarchy);
		circle(imgWithContours, mc[i], 4, randomColor, FILLED);
	}

	namedWindow("Image from file", WINDOW_AUTOSIZE);
	imshow("Image from file", imgWithContours);
	`
		waitKey(0);

	return 0;
}