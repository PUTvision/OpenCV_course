#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include <iostream>

int main(void)
{
	std::cout << "Testing loading images from hard drive" << std::endl;

	Mat image;
	image = imread("C:\\Users\\Public\\Pictures\\Sample Pictures\\desert.jpg", IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	const char windowName[] = "Display window";
	namedWindow(windowName, WINDOW_AUTOSIZE); // Create a window for display.
	imshow(windowName, image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window

	std::cout << "Testing camera capture" << std::endl;

	VideoCapture cap(0);	// open the default camera
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