#include "opencv2/opencv.hpp"

using namespace cv;


static void detectMovement(Mat &newFrame, Mat &background, Mat &mask, int thresh)
{
	Mat temp;
	absdiff(newFrame, background, temp);
	compare(temp, Scalar(thresh), mask, CMP_GE);
}

static void updateBackground(Mat& frame, Mat& background, Mat& isForeground, int method, double alpha)
{
	Mat isBackground = Mat::ones(isForeground.size(), CV_8UC1) * 255 - isForeground;
	switch (method)
	{
	case 0: //last frame
		frame.copyTo(background);
		break;
	case 1: //moving average

		accumulateWeighted(frame, background, alpha);


		break;

	case 2:
		Mat mask1 = (frame>background) / 255;
		Mat mask2 = (frame<background) / 255;
		add(background, mask1, background, isBackground, CV_32FC1);
		subtract(background, mask2, background, isBackground, CV_32FC1);
		break;

	}

}

enum method_t { m_simple, m_avg_median, m_accumulateWeighted };

static void myObjectDetect(void)
{
	Mat currentFrame;
	Mat background;
	Mat foreground;

	const int numberOfImages = 1147; // 283;
	const char fileNamePattern[] = "C://obrazy//sekwencja//p_%04d.pgm"; // "C:/segm1_png/image%03d.png";

	char filename[100];
	sprintf(filename, fileNamePattern, 1);

	background = imread(filename, IMREAD_GRAYSCALE);

	method_t method = m_avg_median;//m_accumulateWeighted;

	const char windowCurrentFrame[] = "currentFrame";
	const char windowBackground[] = "background";
	const char windowForeground[] = "foreground";

	for (int i = 2; i<numberOfImages; ++i)
	{
		sprintf(filename, fileNamePattern, i);
		currentFrame = imread(filename, IMREAD_GRAYSCALE);

		absdiff(currentFrame, background, foreground);

		Mat temp = foreground.clone();
		blur(foreground, temp, Size(9, 9));
		dilate(temp, foreground, Mat(), Point(-1, -1), 5);

		threshold(foreground, foreground, 15, 255, THRESH_BINARY);

		imshow(windowCurrentFrame, currentFrame);
		imshow(windowBackground, background);
		imshow(windowForeground, foreground);

		waitKey(50);

		if (m_simple == method)
		{
			background = currentFrame.clone();
		}
		else if (m_avg_median == method)
		{
			uchar updateValue = 1;

			for (int j = 0; j<currentFrame.rows; ++j)
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
		}
		else if (m_accumulateWeighted == method)
		{
			Mat backgroundF;
			background.convertTo(backgroundF, CV_32F);
			Mat currentFrameF;
			currentFrame.convertTo(currentFrameF, CV_32F);

			accumulateWeighted(currentFrameF, backgroundF, 0.2);

			backgroundF.convertTo(background, CV_8U);
			currentFrameF.convertTo(currentFrame, CV_8U);
		}
	}
}

static void ObjectTracking(void)
{

	char filename[1000];

	Mat background;
	Mat isForeground;
	Mat newFrame;
	Mat newFrameGray;
	Mat newFrameF;
	sprintf(filename, "C:/segm1_png/image%03d.png", 1);
	newFrame = imread(filename, 0);
	newFrame.convertTo(background, CV_32FC1);

	isForeground = Mat::zeros(background.size(), CV_8UC1);



	for (int i = 2; i<283; i++)
	{
		sprintf(filename, "C:/segm1_png/image%03d.png", i);
		newFrame = imread(filename);
		cvtColor(newFrame, newFrameGray, COLOR_BGR2GRAY);
		newFrameGray.convertTo(newFrameF, CV_32FC1);

		detectMovement(newFrameF, background, isForeground, 30);

		updateBackground(newFrameF, background, isForeground, 1, 0.3);
		imshow("image", newFrame);
		imshow("movement", isForeground);
		imshow("background", background / 255.);
		waitKey(50);

	}
}

int main(int, char)
{
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}