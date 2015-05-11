#include "opencv2/opencv.hpp"

using namespace cv;

#include <iostream>

int main(int, char)
{
	Mat imageFromFile;

	// najechanie kursorem na funkcj� wy�wietla pe�en opis wraz z opisem argument�w
	// ctrl + shift + space naci�ni�te pomi�dzy nawiasami () podpowiada nazwy argument�w
	imageFromFile = imread("..\\data\\dog_wizard.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!imageFromFile.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Image from file", WINDOW_AUTOSIZE);
	imshow("Image from file", imageFromFile);

	// ustaw breakpoint na poni�szej linii (F9 lub menu DEBUG->Toogle Breakpoint)
	waitKey(0);
	// gdy program si� zatrzyma uruchomi� okno Image Watch (menu VIEW->Other windows->Image Watch)

	return 0;
}