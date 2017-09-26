#include "opencv2/opencv.hpp"

// Using CeempleCV and OpenCV 3.X

using namespace cv;

#include <iostream>

void Kuwahara_test()
{
	Mat img = imread("..\\data\\kuwahara\\original.png");
	Mat average, variance;
	average.create(img.rows, img.cols, CV_32SC1);
	variance.create(img.rows, img.cols, CV_32SC1);

	if (img.isContinuous())
	{
		int rows = img.rows;
		int cols = img.cols;
		int step = img.step;

		unsigned char *pImg = (unsigned char*)(img.data);

		for (int i = 1; i < rows-1; ++i)
		{
			for (int j = 1; j < cols-1; ++j)
			{
				pImg[step*i + j] = 555;
				pImg[step*i + j + 1] = 555;
				pImg[step*i + j + 2] = 555;
			}
		}
	}
	else
	{
		CV_Error(-666, "Mat is not continous");
	}

	// this checks if the image is continuous and works with color images
	if (img.isContinuous())
	{
		int numberOfChannels = img.channels();
		int numberOfPixels = img.rows * img.cols * numberOfChannels;
		uchar* p = img.data;
		for (int i = 0; i < numberOfPixels; i = i + numberOfChannels)
		{



			for (int j = 0; j < numberOfChannels; ++j)
			{
				p[i + j] = 255 - p[i + j];
			}
		}
	}

}

int main(void)
{
	//std::cout << "Testing loading images from hard drive" << std::endl;

	//Mat image;
	//image = imread("C:\\Users\\Public\\Pictures\\Sample Pictures\\desert.jpg", IMREAD_COLOR); // Read the file

	//if (!image.data) // Check for invalid input
	//{
	//	std::cout << "Could not open or find the image" << std::endl;
	//	return -1;
	//}

	const char windowName[] = "Display window";
	namedWindow(windowName, WINDOW_AUTOSIZE); // Create a window for display.
	//imshow(windowName, image); // Show our image inside it.

	//waitKey(0); // Wait for a keystroke in the window

	std::cout << "Testing camera capture" << std::endl;

	VideoCapture cap(2);	// open the default camera
	if (!cap.isOpened())	// check if we succeeded
	{
		std::cout << "Could not open the camera" << std::endl;
		return -1;
	}

	std::cout << "Press any key to exit" << std::endl;

	for (;;)
	{
		Mat frame;
		cap >> frame;	// get a new frame from camera
		imshow(windowName, frame);
		if (waitKey(30) >= 0)
		{
			break;
		}
	}

	return 0;
}