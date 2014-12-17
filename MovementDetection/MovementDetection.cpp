#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
//#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main(void)
{
	Ptr<BackgroundSubtractorMOG2> pMOG;
	pMOG = createBackgroundSubtractorMOG2();
	Mat currentFrameBGR;
	Mat foregroundFrame;

	VideoCapture capture(0);

	const char windowCurrentFrame[] = "Current frame";
	namedWindow(windowCurrentFrame, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	const char windowForegroundFrame[] = "Foreground frame";
	namedWindow(windowForegroundFrame, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	int key = 'a';
	while (key != 'q')
	{
		capture >> currentFrameBGR;
		//cvtColor(currentFrameBGR, currentFrame, COLOR_BGR2GRAY);
		pMOG->apply(currentFrameBGR, foregroundFrame);

		imshow(windowCurrentFrame, currentFrameBGR);
		imshow(windowForegroundFrame, foregroundFrame);

		key = waitKey(1);
	}

	return 1;
}