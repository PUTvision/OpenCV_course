#include "opencv2/opencv.hpp"

using namespace cv;

#include <iostream>

int main(int, char)
{
	Mat imageFromFile;

	// najechanie kursorem na funkcj� wy�wietla pe�en opis wraz z opisem argument�w
	// ctrl + shift + space naci�ni�te pomi�dzy nawiasami () podpowiada nazwy argument�w
	imageFromFile = imread("..\\data\\zaliczenie_napis.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Image from file", WINDOW_AUTOSIZE);
	imshow("Image from file", imageFromFile);

	waitKey(0); // Wait for a keystroke in the window

	return 0;
}