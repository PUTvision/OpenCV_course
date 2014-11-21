#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int main(void)
{
	vector<string> videosFilenames;
	ifstream file("dane_wejsciowe\\nazwy_sekwencji.txt");
	if (file.is_open())
	{
		string newLine;
		while (getline(file, newLine))
		{
			videosFilenames.push_back(newLine);
			cout << newLine << '\n';
		}
		file.close();
	}

	string videoToLoad = "video\\" + videosFilenames[0];
	VideoCapture capture(videoToLoad);

	if (!capture.isOpened())
	{
		throw "Error when reading steam_avi";
	}

	const char windowName[] = "Display window";
	namedWindow(windowName, WINDOW_AUTOSIZE);		// Create a window for display.

	// versions prior to 3.0 require CV_ prefix before all the properties definitions
	cout << "CV_CAP_PROP_FPS:" << capture.get(CAP_PROP_FPS) << endl;
	cout << "CV_CAP_PROP_FRAME_WIDTH:" << capture.get(CAP_PROP_FRAME_WIDTH) << endl;
	cout << "CV_CAP_PROP_FRAME_HEIGHT:" << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
	cout << "CV_CAP_PROP_FOURCC:" << capture.get(CAP_PROP_FOURCC) << endl;
	cout << "CV_CAP_PROP_FRAME_COUNT:" << capture.get(CAP_PROP_FRAME_COUNT) << endl;

	int keyPressed;
	do
	{
		Mat frame;
		capture >> frame;
		if (frame.empty())
		{
			cout << "Empty frame" << endl;
			throw "Empty frame";
			break;
		}
		imshow(windowName, frame);
		keyPressed = waitKey(20);
	} while (keyPressed != 'q');

	// releases and window destroy are automatic in C++ interface
}