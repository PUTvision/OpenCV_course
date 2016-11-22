#include "opencv2/opencv.hpp"

using namespace cv;

void interpolationTest()
{
	Mat imgSrc = imread("..\\data\\transformations\\qr.jpg", IMREAD_COLOR);
	Mat imgDst;

	/*
	interpolation method:

	INTER_NEAREST - a nearest-neighbor interpolation
	INTER_LINEAR - a bilinear interpolation (used by default)
	INTER_AREA - resampling using pixel area relation. It may be a preferred method for image decimation, as it gives moire’-free results. But when the image is zoomed, it is similar to the INTER_NEAREST method.
	INTER_CUBIC - a bicubic interpolation over 4x4 pixel neighborhood
	INTER_LANCZOS4 - a Lanczos interpolation over 8x8 pixel neighborhood

	To shrink an image, it will generally look best with CV_INTER_AREA interpolation, whereas to enlarge an image, it will generally look best with CV_INTER_CUBIC (slow) or CV_INTER_LINEAR (faster but still looks OK).
	*/

	namedWindow("Image source", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	namedWindow("Image destination", WINDOW_NORMAL | WINDOW_KEEPRATIO);

	int choosenMethod = 0;
	createTrackbar("Interpolation method", "Image source", &choosenMethod, 3);
	int methods[] = { INTER_NEAREST, INTER_LINEAR, INTER_AREA, INTER_CUBIC };
	int choosenScale = 0;
	createTrackbar("Interpolation scale", "Image source", &choosenScale, 6);
	float scales[] = { 0.25, 0.33, 0.5, 0.66, 1.5, 2.0, 2.15 };

	while (waitKey(50) < 0)
	{
		resize(imgSrc, imgDst, Size(), scales[choosenScale], scales[choosenScale], methods[choosenMethod]);

		imshow("Image source", imgSrc);
		imshow("Image destination", imgDst);
	}

	destroyAllWindows();
}

Point2f srcPoints[4];
bool flagFindTransformation = false;

void transformationsMouseCallback(int e, int x, int y, int, void*)
{
	static int currentIndex = 0;

	if (e == EVENT_LBUTTONDOWN)
	{
		srcPoints[currentIndex] = Point(x, y);

		if (currentIndex < 3)
		{
			currentIndex++;
		}
		else
		{
			flagFindTransformation = true;
			currentIndex = 0;
		}
	}
}

void transformationsTest()
{
	Mat imgSrc = imread("..\\data\\transformations\\droga.jpg", IMREAD_COLOR);
	resize(imgSrc, imgSrc, Size(), 0.25, 0.25, INTER_NEAREST);
	Mat imgDst;
	imgSrc.copyTo(imgDst);

	//Point2f srcPoints[] = { Point2f(0, 0), Point2f(imgDst.cols, 0), Point2f(imgDst.cols, imgDst.rows) };
	//Point2f dstPoints[] = { Point2f(10, 10), Point2f(40, 10), Point2f(40, 40) };

	Point2f dstPoints[] = { Point2f(0, 0), Point2f(500, 0), Point2f(500, 500), Point2f(0, 500) };

	namedWindow("Image source");
	setMouseCallback("Image source", transformationsMouseCallback);
	namedWindow("Image destination");

	while (waitKey(50) < 0)
	{
		if (flagFindTransformation)
		{
			//Mat affineTransform = getAffineTransform(srcPoints, dstPoints);
			Mat perspectiveTransform = getPerspectiveTransform(srcPoints, dstPoints);
			//warpAffine(imgSrc, imgDst, affineTransform, imgDst.size());
			warpPerspective(imgSrc, imgDst, perspectiveTransform, Size(500, 500));

			flagFindTransformation = false;
		}

		imshow("Image source", imgSrc);
		imshow("Image destination", imgDst);
	}

}

int main(int, char)
{
	interpolationTest();
	transformationsTest();

	return 0;
}