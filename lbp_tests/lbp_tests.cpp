#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>

#include "lbp.hpp"

class MF_StopWatch
{
private:
	double startTime;

public:
	MF_StopWatch(void)
	{
		this->startTime = 0.0;
	}

	void Start(void)
	{
		startTime = (double)cv::getTickCount();
	}

	double GetElapsedTimeInMiliseconds(void)
	{
		double elapsedTime = (double)cv::getTickCount() - startTime;
		elapsedTime = elapsedTime / cv::getTickFrequency();

		return (elapsedTime * 1000);
	}
};

void calculateHistogramsForImage(Mat& src)
{
	const int histogramWidth = 6;
	const int histogramHeight = 3;

	int width = src.size().width;
	int height = src.size().height;

	/// Establish the number of bins
	int histSize = 256;
	/// Set the ranges
	float range[] = { 0, (float)(histogramWidth*histogramHeight) };
	const float* histRange = { range };

	for (int i = 0; i < (height - histogramHeight); ++i)
	{
		for (int j = 0; j < (width - histogramWidth); ++j)
		{
			Mat patch = Mat(src, Range(i, i + histogramHeight), Range(j, j + histogramWidth));
			Mat result;
			calcHist(&patch, 1, 0, Mat(), result, 1, &histSize, &histRange);
		}
	}

	//std::cout << result << std::endl << std::endl;
}

int main(void)
{
	Mat image;
	image = imread("C:\\Users\\Public\\Pictures\\Sample Pictures\\desert.jpg", IMREAD_GRAYSCALE); // Read the file
	if (!image.data) // Check for invalid input
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	const int width = 640;
	const int height = 480;
	 
	Mat imageResized;
	resize(image, imageResized, Size(width, height));

	Mat imageAfterLBP = imageResized.clone();

	MF_StopWatch sw;

	sw.Start();

	libfacerec::olbp(imageResized, imageAfterLBP);

	double elapsedTimeInMs = sw.GetElapsedTimeInMiliseconds();

	std::cout << "elapsedTimeInMs LBP: " << elapsedTimeInMs << std::endl;

	//Mat hist = libfacerec::spatial_histogram(imageAfterLBP, 256, 1024, 768, true);

	//Mat hist = libfacerec::spatial_histogram(imageAfterLBP, 256, 512, 384, true);

	sw.Start();

	calculateHistogramsForImage(imageAfterLBP);

	elapsedTimeInMs = sw.GetElapsedTimeInMiliseconds();

	std::cout << "elapsedTimeInMs histograms: " << elapsedTimeInMs << std::endl;

	const char windowName[] = "Display window";
	namedWindow(windowName, WINDOW_AUTOSIZE); // Create a window for display.
	imshow(windowName, imageAfterLBP); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window

	return 0;
}