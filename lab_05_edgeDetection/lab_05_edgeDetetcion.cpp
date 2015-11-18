#include "opencv2/opencv.hpp"

// Using CeempleCVand OpenCV 3.X

using namespace cv;

void Prewitt_mask(void)
{
	Mat imageFromFile;

	imageFromFile = imread("..\\data\\lena.jpg", IMREAD_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
	}
	else
	{
		Mat prewitt_horizontal = Mat::zeros(3, 3, CV_32F);
		prewitt_horizontal.row(0).setTo(-1);
		prewitt_horizontal.row(2).setTo(1);
		std::cout << "prewitt_horizontal: " << std::endl << prewitt_horizontal << std::endl;

		Mat prewitt_vertical = Mat::zeros(3, 3, CV_32F);
		prewitt_vertical.col(0).setTo(-1);
		prewitt_vertical.col(2).setTo(1);
		std::cout << "prewitt_vertical: " << std::endl << prewitt_vertical << std::endl;

		Mat img_horizontal, img_vertical;
		filter2D(imageFromFile, img_horizontal, CV_32F, prewitt_horizontal);
		img_horizontal /= 3;
		pow(img_horizontal, 2, img_horizontal);

		filter2D(imageFromFile, img_vertical, CV_32F, prewitt_vertical);
		img_vertical /= 3;
		pow(img_vertical, 2, img_vertical);

		Mat img_prewitt;
		sqrt(img_horizontal + img_vertical, img_prewitt);
		img_prewitt.convertTo(img_prewitt, CV_8U);

		namedWindow("Prewitt mask", WINDOW_AUTOSIZE);
		imshow("Prewitt mask", img_prewitt);

		waitKey(0);
	} 
}

void Sobel_mask(void)
{
	Mat imageFromFile;

	imageFromFile = imread("..\\data\\lena.jpg", IMREAD_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
	}
	else
	{
		Mat sobel_horizontal = (Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
		std::cout << "sobel_horizontal: " << std::endl << sobel_horizontal << std::endl;

		Mat sobel_vertical = (Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		std::cout << "sobel_vertical: " << std::endl << sobel_vertical << std::endl;

		Mat img_horizontal, img_vertical;
		filter2D(imageFromFile, img_horizontal, CV_32F, sobel_horizontal);
		img_horizontal /= 3;
		pow(img_horizontal, 2, img_horizontal);

		filter2D(imageFromFile, img_vertical, CV_32F, sobel_vertical);
		img_vertical /= 3;
		pow(img_vertical, 2, img_vertical);

		Mat img_sobel;
		sqrt(img_horizontal + img_vertical, img_sobel);
		img_sobel.convertTo(img_sobel, CV_8U);

		namedWindow("Sobel mask", WINDOW_AUTOSIZE);
		imshow("Sobel mask", img_sobel);

		waitKey(0);
	}
}

int main(int, char)
{
	Prewitt_mask();
	Sobel_mask();

	// TODO:
	// Canny_test()
	// Hough_test()

	return 0;
}