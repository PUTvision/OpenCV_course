#include "opencv2/opencv.hpp"

using namespace cv;

void interpolationTest(void)
{
	Mat imgSrc = imread("..\\data\\programmers_tan.jpg", IMREAD_COLOR);
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

	resize(imgSrc, imgDst, Size(), 0.5, 0.5, INTER_NEAREST);
	//resize(imgSrc, imgDst, Size(2000, 2000), 0.0, 0.0, INTER_CUBIC);

	Point2f srcPoints[] = { Point2f(0, 0), Point2f(imgDst.cols, 0), Point2f(imgDst.cols, imgDst.rows) };
	Point2f dstPoints[] = { Point2f(10, 10), Point2f(40, 10), Point2f(40, 40) };


	Point2f srcPointsPerspective[] = { Point2f(10, 60), Point2f(120, 10), Point2f(120, 120), Point2f(10, 170) };
	Point2f dstPointsPerspective[] = { Point2f(0, 0), Point2f(imgDst.cols, 0), Point2f(imgDst.cols, imgDst.rows), Point2f(0, imgDst.rows) };

	Mat affineTransform = getAffineTransform(srcPoints, dstPoints);
	Mat perspectiveTransform = getPerspectiveTransform(srcPointsPerspective, dstPointsPerspective);
	//warpAffine(imgSrc, imgDst, affineTransform, Size(300, 300));
	warpPerspective(imgSrc, imgDst, perspectiveTransform, Size(imgDst.cols, imgDst.rows));

	namedWindow("Image source");
	namedWindow("Image destination");
	imshow("Image source", imgSrc);
	imshow("Image destination", imgDst);

	waitKey(0);

}

int main(int, char)
{
	interpolationTest();

	return 0;
}