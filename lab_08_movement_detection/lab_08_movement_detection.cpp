#include <opencv2/opencv.hpp>

// vcpkg OpenCV 3.3.X

void BackgroundSubtractorMOG2()
{
	cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG;
	pMOG = cv::createBackgroundSubtractorMOG2();
	cv::Mat currentFrameBGR;
	cv::Mat foregroundFrame;

	cv::VideoCapture capture(0);

	const char windowCurrentFrame[] = "Current frame";
	cv::namedWindow(windowCurrentFrame, cv::WINDOW_AUTOSIZE | cv::WINDOW_KEEPRATIO);

	const char windowForegroundFrame[] = "Foreground frame";
	cv::namedWindow(windowForegroundFrame, cv::WINDOW_AUTOSIZE | cv::WINDOW_KEEPRATIO);

	int key = 'a';
	while (key != 'q')
	{
		capture >> currentFrameBGR;
		pMOG->apply(currentFrameBGR, foregroundFrame);

		imshow(windowCurrentFrame, currentFrameBGR);
		imshow(windowForegroundFrame, foregroundFrame);

		key = cv::waitKey(1);
	}
}

static void detectMovement(const cv::Mat &newFrame, const cv::Mat &background, const cv::Mat &mask, int thresh)
{
	cv::Mat temp;
	absdiff(newFrame, background, temp);
	compare(temp, cv::Scalar(thresh), mask, cv::CMP_GE);
}

enum class Method { SIMPLE, AVG_MEDIAN, ACCUMULATE_WEIGHTED };

void alternativeAvgMedianImplementation(cv::Mat &currentFrame, cv::Mat &background, int updateValue)
{
	uchar* backgroundCurrentRow = background.ptr<uchar>(j);
	const uchar* currentFrameCurrentRow = currentFrame.ptr<uchar>(j);

	for (int k = 0; k<currentFrame.cols; ++k)
	{
		if (backgroundCurrentRow[k] > currentFrameCurrentRow[k])
		{
			backgroundCurrentRow[k] -= updateValue;
		}
		else if (backgroundCurrentRow[k] < currentFrameCurrentRow[k])
		{
			backgroundCurrentRow[k] += updateValue;
		}
	}
}

static void updateBackground(const cv::Mat &frame, cv::Mat &background, const cv::Mat &isForeground, Method method, double alpha)
{
	cv::Mat isBackground = cv::Mat::ones(isForeground.size(), CV_8UC1) * 255 - isForeground;
	switch (method)
	{
		case Method::SIMPLE: //last frame
			frame.copyTo(background);
			break;
		case Method::ACCUMULATE_WEIGHTED: //moving average
			accumulateWeighted(frame, background, alpha);
			break;
		case Method::AVG_MEDIAN:
			cv::Mat maskAdd = (frame>background) / 255;
			cv::Mat maskSubtract = (frame<background) / 255;
			add(background, maskAdd, background, isBackground, CV_32FC1);
			subtract(background, maskSubtract, background, isBackground, CV_32FC1);
			
			// this is another way to update background
			//alternativeAvgMedianImplementation(frame, background, 1);

			break;
	}
}

static void ObjectTracking(void)
{
	cv::VideoCapture cap(0);// "..\\data\\video\\dt_passat.mpg");

	cv::Mat background;
	cv::Mat isForeground;
	cv::Mat newFrame;
	cv::Mat newFrameGray;
	cv::Mat newFrameF;
	cap >> newFrame;
	cvtColor(newFrame, newFrameGray, cv::COLOR_BGR2GRAY);
	newFrameGray.convertTo(background, CV_32FC1);

	isForeground = cv::Mat::zeros(background.size(), CV_8UC1);

	for (;;)
	{
		cap >> newFrame;
		cvtColor(newFrame, newFrameGray, cv::COLOR_BGR2GRAY);
		// this line is used instead of cvtColor when working on grayscale video
		//newFrame.copyTo(newFrameGray);
		newFrameGray.convertTo(newFrameF, CV_32FC1);

		detectMovement(newFrameF, background, isForeground, 30);

		updateBackground(newFrameF, background, isForeground, Method::AVG_MEDIAN, 0.3);
		imshow("image", newFrame);
		imshow("movement", isForeground);
		imshow("background", background / 255.);
		cv::waitKey(50);
	}

	cap.release();
}

int main(void)
{
	ObjectTracking();

	BackgroundSubtractorMOG2();

	return 1;
}
