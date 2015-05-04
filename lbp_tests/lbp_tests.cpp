#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#include "lbp.hpp"

int main(void)
{
	Mat image;
	image = imread("C:\\Users\\Public\\Pictures\\Sample Pictures\\desert.jpg", IMREAD_GRAYSCALE); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat imageAfterLBP;
	libfacerec::olbp(image, imageAfterLBP);

	const char windowName[] = "Display window";
	namedWindow(windowName, WINDOW_AUTOSIZE); // Create a window for display.
	imshow(windowName, imageAfterLBP); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window

	return 0;
}