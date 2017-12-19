#include "opencv2/opencv.hpp"

// vcpkg OpenCV 3.3.X
using namespace cv;

static void TrackbarFunction(int, void*)
{
}

int main(int, char)
{
	char* windowName = "Main window";
	namedWindow(windowName, WINDOW_AUTOSIZE);

	// trackbar parameters
	const int maxValue = 255;
	const int  maxType = 4;

	int thresholdValue = 44;
	int thresholdType = 0;

	char* trackbarType = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
	char* trackbarValue = "Value";

	/// Create Trackbar to choose type and value
	createTrackbar(trackbarType, windowName, &thresholdType, maxType, TrackbarFunction);
	createTrackbar(trackbarValue, windowName, &thresholdValue, maxValue, TrackbarFunction);

	Mat imgFromFile = imread("..\\data\\structural_analysisis_and_contour_detection\\not_bad.jpg", IMREAD_GRAYSCALE);

	Mat imgSrc;
	resize(imgFromFile, imgSrc, Size(), 0.25, 0.25, INTER_NEAREST);

	RNG rng(12345);

	int key = 'z';
	while (key != 32)		// 32 == space
	{
		Mat imgThresholded;

		key = 'z';
		while (key != 32)	// 32 == space
		{
			threshold(imgSrc, imgThresholded, thresholdValue, 255, thresholdType);
			//dilate(imgThresholded, imgThresholded, Mat());
			erode(imgThresholded, imgThresholded, Mat());

			imshow(windowName, imgThresholded);

			key = waitKey(0);
		}

		std::vector<std::vector<Point>> contours;
		std::vector<Vec4i> hierarchy;

		// http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html#find-contours
		Mat imgForContoursProcessing = imgThresholded.clone();
		findContours(imgForContoursProcessing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		/// Get the moments
		std::vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}

		///  Get the mass centers:
		std::vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f((float)(mu[i].m10 / mu[i].m00), (float)(mu[i].m01 / mu[i].m00));
		}

		Scalar tableOfColors[5];
		// BGR
		tableOfColors[0] = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		tableOfColors[1] = Scalar(255, 0, 0);
		tableOfColors[2] = Scalar(0, 255, 0);
		tableOfColors[3] = Scalar(0, 0, 255);
		tableOfColors[4] = Scalar(255, 255, 255);

		/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
		printf("\t Info: Area and Contour Length \n");
		Mat imgWithContours = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
		for (int i = 0; i<contours.size(); ++i)
		{
			printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
			drawContours(imgWithContours, contours, i, tableOfColors[i], 1, 8, hierarchy);
			circle(imgWithContours, mc[i], 4, tableOfColors[i], FILLED);
		}

		imshow(windowName, imgWithContours);

		key = 'z';
		key = waitKey(0);

		Mat imgFinale = Mat::zeros(Size(640, 480), CV_8UC3);

		Point2f srcPointsPerspective[4];
		Point2f dstPointsPerspective[4];

		dstPointsPerspective[0] = Point2f(0.0F, 0.0F);
		dstPointsPerspective[1] = Point2f((float)(imgFinale.cols - 1), 0.0F);
		dstPointsPerspective[2] = Point2f((float)(imgFinale.cols - 1), (float)(imgFinale.rows - 1));
		dstPointsPerspective[3] = Point2f(0.0F, (float)(imgFinale.rows - 1));

		srcPointsPerspective[0] = mc[4];
		srcPointsPerspective[1] = mc[3];
		srcPointsPerspective[2] = mc[1];
		srcPointsPerspective[3] = mc[2];

		Mat perspectiveTransform = getPerspectiveTransform(srcPointsPerspective, dstPointsPerspective);
		warpPerspective(imgSrc, imgFinale, perspectiveTransform, imgFinale.size(), INTER_LINEAR, BORDER_TRANSPARENT);

		imshow(windowName, imgFinale);

		key = 'z';
		key = waitKey(0);
	}

	return 0;
}