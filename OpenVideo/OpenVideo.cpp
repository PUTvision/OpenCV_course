#include "opencv2/opencv.hpp"

// Using CeempleCVand OpenCV 3.X

using namespace cv;

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

static void PrintVideoProperties(VideoCapture &capture)
{
	// versions prior to 3.0 require CV_ prefix before all the properties definitions
	std::cout << "CV_CAP_PROP_FPS:" << capture.get(CAP_PROP_FPS) << std::endl;
	std::cout << "CV_CAP_PROP_FRAME_WIDTH:" << capture.get(CAP_PROP_FRAME_WIDTH) << std::endl;
	std::cout << "CV_CAP_PROP_FRAME_HEIGHT:" << capture.get(CAP_PROP_FRAME_HEIGHT) << std::endl;
	std::cout << "CV_CAP_PROP_FOURCC:" << capture.get(CAP_PROP_FOURCC) << std::endl;
	std::cout << "CV_CAP_PROP_FRAME_COUNT:" << capture.get(CAP_PROP_FRAME_COUNT) << std::endl;
}

// this function can be used for cameras too, just use different contructor
// eg. cap(0);
void OpenVideo(void)
{
	const char windowName[] = "Video window";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);		// Create a window for display.

	VideoCapture cap("..\\data\\video\\dt_passat.mpg"); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
	{
		throw "Error when reading video file!";
	}

	PrintVideoProperties(cap);

	int keyPressed;
	do
	{
		Mat frame;
		cap >> frame;

		// the other way to do it:
		// 1)
		// zatrzasniecie klatki wewnatrz kamery bez jej odzyskania
		// szybkie, pozwala na synchronizacje kamer:
		// cap.grab();
		// odzyskanie zatrzasnietej klatki:
		// cap.retrieve(frame);
		// 2)
		// obie powyzsze operacje:
		// cap.read(frame);

		if (frame.empty())
		{
			std::cout << "Empty frame" << std::endl;
			throw "Empty frame";
			break;
		}
		imshow(windowName, frame);
		keyPressed = waitKey(20);
	} while (keyPressed != 'q');

	// the camera will be deinitialized automatically in VideoCapture destructor
	// releases and window destroy are automatic in C++ interface
}

int main(void)
{
	OpenVideo();


	return 0;
}