#include "opencv2/opencv.hpp"

using namespace cv;

#include <cstdint>		// for uint8_t type
#include <iostream>

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

static inline void PSEyeTest()
{
	VideoCapture capture2(0);
	VideoCapture capture(0);
	Mat frame;

	capture.set(CAP_PROP_FRAME_WIDTH, 1280);
	capture.set(CAP_PROP_FRAME_HEIGHT, 720);

	// 2.4.8
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);



	const char windowName[] = "PS Eye";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	for (;;)
	{
		capture >> frame;

		imshow(windowName, frame);

		waitKey(0);
	}
}

int testWejsciowka()
{

	Mat image(200, 480, CV_8UC1);
	typedef cv::Point3_<uint8_t> Pixel;

	Mat boost;
	// czemu nie da siê skompilowaæ decolor?
	//decolor(frame, grayscale, boost);

	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat obrazek_z_pliku;

	cap >> obrazek_z_pliku; // get a new frame from camera

	//Mat obrazek_z_pliku = imread("Desert.jpg", 1);
	resize(obrazek_z_pliku, obrazek_z_pliku, Size(640, 480));
	cvtColor(obrazek_z_pliku, obrazek_z_pliku, COLOR_BGR2GRAY);
	namedWindow("image");

	Mat obrazek_z_pliku2 = obrazek_z_pliku.clone();


	int numberOfElemnts = obrazek_z_pliku2.rows;

	for (int j = 0; j < numberOfElemnts / 2; ++j)
	{
		obrazek_z_pliku.row(j + numberOfElemnts / 2).copyTo(obrazek_z_pliku2.row(j));
		//obrazek_z_pliku.row(j).copyTo(obrazek_z_pliku2.row(j + numberOfElemnts / 2));

		//obrazek_z_pliku2.row(j) = obrazek_z_pliku.row(j + numberOfElemnts / 2).clone();
		obrazek_z_pliku2.row(j + numberOfElemnts / 2) = obrazek_z_pliku.row(j).clone();
	}



	for (;;)
	{
		imshow("image", obrazek_z_pliku2);

		if (waitKey(30) >= 0)
			break;
	}
	//waitKey(0);

	return 0;
}

//zmienne globalne:
const int trackbarValueMax = 255;
int trackbarValue;

Mat src;
Mat dst;

void on_trackbar(int, void*)
{
	threshold(src, dst, (double)trackbarValue, (double)trackbarValueMax, THRESH_BINARY);

	imshow("okno", dst);
}

static inline void interpolationTest(void)
{
	Mat imgSrc = imread("resources//programmers_tan.jpg", IMREAD_COLOR);
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
	warpAffine(imgSrc, imgDst, affineTransform, Size(300, 300));
	//warpPerspective(imgSrc, imgDst, perspectiveTransform, Size(imgDst.cols, imgDst.rows));

	namedWindow("Image source");
	namedWindow("Image destination");
	imshow("Image source", imgSrc);
	imshow("Image destination", imgDst);

	waitKey(0);

}

#include "blur.h"

static inline void BlurTest(void)
{
	Mat src = imread("resources//programmers_tan.jpg", IMREAD_GRAYSCALE);
	Mat dst = src.clone();
	const int numberOfIterations = 10000;

	int numberOfElements = src.cols * src.rows;
	//uchar *pResult = (uchar*) _aligned_malloc(length * sizeof(uchar), 16);  // align to 16-byte for SSE

	MF_StopWatch sw;

	//src = (Mat_<uchar>(5,4) << 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54);
	//dst = (Mat_<uchar>(5,4) << 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54);

	//std::cout<<src<<std::endl;
	//std::cout<<dst<<std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		BlurSuperFastCorreceted2(src, dst);
	}
	std::cout << std::endl << "BlurSuperFastCorreceted2 code: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		BlurSuperFastCorreceted(src, dst);
	}
	std::cout << std::endl << "BlurSuperFastCorreceted code: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	namedWindow("Original", 1);
	namedWindow("Blur", 1);
	imshow("Original", src);
	imshow("Blur", dst);

	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		BlurSuperFast(src, dst);
	}
	std::cout << std::endl << "BlurSuperFast code: " << sw.GetElapsedTimeInMiliseconds() << std::endl;


	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		blur(src, dst, Size(3, 3));
	}
	std::cout << std::endl << "Built-in blur: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		BlurFast(src, dst);
	}
	std::cout << std::endl << "Wiktor original code: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		blurWiktor(src, dst);
	}
	std::cout << std::endl << "Wiktor's blur: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		blurTwoPass(src, dst);
	}
	std::cout << std::endl << "TwoPass blur: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	//for(int i=numberOfIterations/10; i!=0; --i)
	//{
	//blur(src, dst);
	//}
	sw.Start();
	for (int i = 0; i<numberOfIterations; ++i)
	{
		blur(src, dst);
	}
	std::cout << std::endl << "My blur: " << sw.GetElapsedTimeInMiliseconds() << std::endl;



	waitKey(0);
}

void ErodeTest()
{
	Mat src = imread("resources//programmers_tan.jpg", IMREAD_GRAYSCALE);

	Mat kernel;
	kernel.create(3, 3, CV_8UC1);
	kernel.setTo(1);

	erode(src, src, kernel);

	namedWindow("Original", 1);
	imshow("Original", src);
	waitKey(0);
}

void MedianTest()
{
	Mat src = imread("resources//programmers_tan.jpg", IMREAD_GRAYSCALE);
	Mat dst = src.clone();

	medianBlur(src, dst, 2);

	namedWindow("Processed image", 1);
	imshow("Processed image", dst);
	waitKey(0);
}

static void BlendingTwoImages(void)
{
	Mat img1 = imread("resources//programmers_tan.jpg", IMREAD_GRAYSCALE);
	Mat img2 = imread("resources//opencv_logo.png", IMREAD_GRAYSCALE);
	Mat imgResult;

	double alpha = 0.5;
	double beta = 1.0 - alpha;

	addWeighted(img1, alpha, img2, beta, 0.0, imgResult);

	namedWindow("Linear Blend", 1);
	imshow("Linear Blend", imgResult);

	waitKey(0);
}

static inline Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	int channels = I.channels();

	int nRows = I.rows * channels;
	int nCols = I.cols;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			p[j] = table[p[j]];
		}
	}
	return I;
}

static inline Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	const int channels = I.channels();
	switch (channels)
	{
	case 1:
	{
		MatIterator_<uchar> it, end;
		for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
			*it = table[*it];
		break;
	}
	case 3:
	{
		MatIterator_<Vec3b> it, end;
		for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
		{
			(*it)[0] = table[(*it)[0]];
			(*it)[1] = table[(*it)[1]];
			(*it)[2] = table[(*it)[2]];
		}
	}
	}

	return I;
}

static inline Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	const int channels = I.channels();
	switch (channels)
	{
	case 1:
	{
		for (int i = 0; i < I.rows; ++i)
			for (int j = 0; j < I.cols; ++j)
				I.at<uchar>(i, j) = table[I.at<uchar>(i, j)];
		break;
	}
	case 3:
	{
		Mat_<Vec3b> _I = I;

		for (int i = 0; i < I.rows; ++i)
			for (int j = 0; j < I.cols; ++j)
			{
				_I(i, j)[0] = table[_I(i, j)[0]];
				_I(i, j)[1] = table[_I(i, j)[1]];
				_I(i, j)[2] = table[_I(i, j)[2]];
			}
		I = _I;
		break;
	}
	}

	return I;
}

static inline void ScanImageAndReduceTest(void)
{
	int divideWith = 50; // convert our input string to number - C++ style

	uchar table[256];
	for (int i = 0; i < 256; ++i)
		table[i] = divideWith* (i / divideWith);

	Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.data;
	for (int i = 0; i < 256; ++i)
		p[i] = table[i];

	Mat img = imread("grass.jpg", IMREAD_GRAYSCALE);
	Mat imgResult = img.clone();
	const int numberOfIterations = 100;

	MF_StopWatch sw;

	sw.Start();
	for (int i = 0; i<numberOfIterations; i++)
	{
		ScanImageAndReduceC(img.clone(), table);
	}
	std::cout << std::endl << "DirectMemoryAccess: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; i++)
	{
		ScanImageAndReduceIterator(img.clone(), table);
	}
	std::cout << std::endl << "Iterator: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; i++)
	{
		ScanImageAndReduceRandomAccess(img.clone(), table);
	}
	std::cout << std::endl << "RandomAccess: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	sw.Start();
	for (int i = 0; i<numberOfIterations; i++)
	{
		LUT(img.clone(), lookUpTable, imgResult);
	}
	std::cout << std::endl << "LUT: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

	system("pause");
}

static void ShowValues(int &intVal, double &doubleVal, string &stringVal, Mat &mat)
{
	std::cout << "intVal: " << intVal << std::endl;
	std::cout << "sampleName: " << doubleVal << std::endl;
	std::cout << "stringValue: " << stringVal << std::endl;
	std::cout << "Mat: " << mat << std::endl;
	system("pause");
	std::cout << std::endl;
}

static void FileInputOutputTest(void)
{
	FileStorage fs;

	int intValue = 1337;
	double doubleValue = 6.66;
	string stringValue = "Hello OpenCV";
	Mat A;
	A.create(3, 3, CV_8UC1);
	A.setTo(Scalar(128));

	ShowValues(intValue, doubleValue, stringValue, A);

	fs.open("data.xml", FileStorage::WRITE);

	fs << "intVal" << intValue;
	fs << "sampleName" << doubleValue;
	fs << "stringValue" << stringValue;
	fs << "MatA" << A;

	fs.release();

	intValue = 0;
	doubleValue = 9.99;
	stringValue = "Error";
	A.setTo(Scalar(0));

	ShowValues(intValue, doubleValue, stringValue, A);

	fs.open("data.xml", FileStorage::READ);

	intValue = (int)fs["intValue"];
	fs["sampleName"] >> doubleValue;
	stringValue = (string)fs["stringValue"];
	fs["MatA"] >> A;

	fs.release();

	ShowValues(intValue, doubleValue, stringValue, A);
}

int G_thresholdValue = 54;
int G_thresholdType = 0;

Point2f G_srcPointsPerspective[4];
Point2f G_dstPointsPerspective[4];

static inline void AdamDance(void)
{
	const int numberOfFrames = 4;

	Mat frames[numberOfFrames];
	for (int i = 0; i<numberOfFrames; ++i)
	{
		char filename[20];
		sprintf(filename, "frame%d.bmp", i + 1);
		frames[i] = imread(filename, IMREAD_GRAYSCALE);
	}

	const char* windowName = "Adam dance";

	namedWindow(windowName, WINDOW_AUTOSIZE);

	const int delayBetweenFrames = 100;

	const int numberOfElementsInFrameSequence = 7;
	int frameSequenceTable[numberOfElementsInFrameSequence] = { 1, 2, 3, 4, 3, 2, 1 };

	int currentSequenceIndex = 0;

	int key = 'z';
	while (key != 32)
	{
		imshow(windowName, frames[frameSequenceTable[currentSequenceIndex] - 1]);
		key = waitKey(delayBetweenFrames);

		currentSequenceIndex++;
		if (currentSequenceIndex >= numberOfElementsInFrameSequence)
		{
			currentSequenceIndex = 0;
		}
	}
}

static inline void FindMarkersAndTransform(int& thresholdValue, int& thresholdType, char* windowName)
{
	Mat imgSrc = imread("img_0_5_marker.bmp", IMREAD_GRAYSCALE);
	Mat imgBackground = imread("television.jpg", IMREAD_GRAYSCALE);

	RNG rng(12345);

	thresholdValue = 254;

	Mat imgThresholded;

	int key = 'z';
	while (key != 32)
	{
		key = 'z';
		imshow(windowName, imgBackground);
		while (key != 32)	// 32 == space
		{
			key = waitKey();
		}

		key = 'z';
		while (key != 32)
		{
			threshold(imgSrc, imgThresholded, thresholdValue, 255, thresholdType);

			imshow(windowName, imgThresholded);

			key = waitKey(0);
		}

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;

		Mat imgForContoursProcessing = imgThresholded.clone();
		findContours(imgForContoursProcessing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Get the moments
		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}

		///  Get the mass centers:
		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
		printf("\t Info: Area and Contour Length \n");
		Mat imgWithContours = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
		for (int i = 0; i<contours.size(); ++i)
		{
			printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(imgWithContours, contours, i, color, 1, 8, hierarchy);
			circle(imgWithContours, mc[i], 4, color, FILLED);
		}

		for (int i = 0; i<contours.size(); ++i)
		{
			G_srcPointsPerspective[i] = mc[i];
		}

		Mat perspectiveTransform = getPerspectiveTransform(G_srcPointsPerspective, G_dstPointsPerspective);
		warpPerspective(imgSrc, imgBackground, perspectiveTransform, imgBackground.size(), INTER_LINEAR, BORDER_TRANSPARENT);

		imshow(windowName, imgBackground);

		key = waitKey(0);

		if ('s' == key)
		{
			imwrite("frame4.bmp", imgBackground);
		}
	}
}

static inline void ThresholdTest(int& thresholdValue, int& thresholdType, char* windowName)
{
	Mat imgSrc = imread("shapes.png", IMREAD_GRAYSCALE);
	//imread("4squares.png", CV_LOAD_IMAGE_GRAYSCALE);

	RNG rng(12345);

	char* windowImageSrc = "Source image";
	namedWindow(windowImageSrc, WINDOW_AUTOSIZE);
	imshow(windowImageSrc, imgSrc);

	int key = 's';
	while (key != 'q')
	{
		Mat imgThresholded;
		threshold(imgSrc, imgThresholded, thresholdValue, 255, thresholdType);

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;

		// CV_CHAIN_APPROX_NONE stores absolutely all the contour points. That is, any 2 subsequent points (x1,y1) and (x2,y2) of the contour will be either horizontal, vertical or diagonal neighbors, that is, max(abs(x1-x2),abs(y2-y1))==1.
		// CV_CHAIN_APPROX_SIMPLE compresses horizontal, vertical, and diagonal segments and leaves only their end points. For example, an up-right rectangular contour is encoded with 4 points.
		// CV_CHAIN_APPROX_TC89_L1, CV_CHAIN_APPROX_TC89_KCOS

		Mat imgForContoursProcessing = imgThresholded.clone();
		findContours(imgForContoursProcessing, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));

		Mat imgWithContours1 = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
		// iterate through all the top-level contours, draw each connected component with its own random color
		for (int i = 0; i >= 0; i = hierarchy[i][0])
		{
			Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(imgWithContours1, contours, i, color, 2, 8, hierarchy);
			// CV_FILLED = -1 means draw the insides
		}

		// http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html#find-contours
		imgForContoursProcessing = imgThresholded.clone();
		findContours(imgForContoursProcessing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		Mat imgWithContours2 = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
		for (int i = 0; i<contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(imgWithContours2, contours, i, color, FILLED, 8, hierarchy);
		}


		// http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/hull/hull.html#hull
		/// Find the convex hull object for each contour
		vector<vector<Point> > hull(contours.size());
		for (int i = 0; i<contours.size(); i++)
		{
			convexHull(Mat(contours[i]), hull[i]);
		}

		/// Draw contours + hull results
		Mat imgWithConvexHull = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(imgWithConvexHull, contours, i, color, 1, 8);
			drawContours(imgWithConvexHull, hull, i, color, 1, 8);
		}

		// http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/bounding_rotated_ellipses/bounding_rotated_ellipses.html#bounding-rotated-ellipses
		/// Find the rotated rectangles and ellipses for each contour
		vector<RotatedRect> minRect(contours.size());
		vector<RotatedRect> minEllipse(contours.size());

		// http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html#bounding-rects-circles
		/// Approximate contours to polygons + get bounding rects and circles
		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>center(contours.size());
		vector<float>radius(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			minRect[i] = minAreaRect(Mat(contours[i]));
			if (contours[i].size() > 5)
			{
				minEllipse[i] = fitEllipse(Mat(contours[i]));
			}

			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		}

		/// Draw contours + rotated rects + ellipses
		Mat imgWithBoundingElements = Mat::zeros(imgForContoursProcessing.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			// contour
			drawContours(imgWithBoundingElements, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			// ellipse
			ellipse(imgWithBoundingElements, minEllipse[i], color, 2, 8);
			// rotated rectangle
			Point2f rect_points[4]; minRect[i].points(rect_points);
			for (int j = 0; j < 4; j++)
			{
				line(imgWithBoundingElements, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
			}
			rectangle(imgWithBoundingElements, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
			circle(imgWithBoundingElements, center[i], (int)radius[i], color, 2, 8, 0);
		}

		imshow(windowName, imgThresholded);
		imshow(windowName, imgWithContours2);
		imshow(windowName, imgWithConvexHull);
		imshow(windowName, imgWithBoundingElements);

		key = waitKey(0);
	}
}

static void TrackbarFunction(int, void*)
{
}

static void onMouse(int e, int x, int y, int, void*)
{
	static int currentIndex = 0;

	if (e == EVENT_LBUTTONDOWN)
	{
		G_dstPointsPerspective[currentIndex] = Point(x, y);

		if (currentIndex < 3)
		{
			currentIndex++;
		}
		else
		{
			currentIndex = 0;
		}
	}
}

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

#if 0
static void dynamicTableTest(void)
{
	int rozmiar;
	cin >> rozmiar;
	int tab[rozmiar];

	for (int i = 0; i<rozmiar; i++)
	{
		cin >> tab[i];
	}

	for (int i = 0; i<rozmiar; i++)
	{
		std::cout << tab[i] << std::endl;
	}
}
#endif

using namespace std;

struct test
{
	int a;
	int b;
};

#include <stack>
#include <queue>

static void testQueueStack(void)
{
	int value;

	std::stack<int> stos;
	stos.empty();
	stos.push(5);
	stos.size();
	value = stos.top();
	stos.pop();

	test valueTest;

	std::queue<test> kolejka;
	kolejka.empty();
	kolejka.push(valueTest);
	kolejka.size();
	valueTest = kolejka.front();
	valueTest = kolejka.back();
	kolejka.pop();
}

static void loadPGMfile(void)
{
	Mat image = imread("C://obrazy//sekwencja//p_0020.pgm", IMREAD_UNCHANGED);

	const char windowName[] = "Image from file";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	imshow(windowName, image);
	waitKey(0);
}

static void zelki(void)
{
	Mat matrix;
	matrix = (Mat_<uchar>(3, 3) << 11, 12, 13, 21, 22, 23, 31, 32, 33);
	matrix = matrix & matrix;
}

typedef unsigned char	uint8_t;
typedef	unsigned short	uint16_t;
typedef unsigned int	uint32_t;

typedef uint16_t		used_type_t;

#define NUMBER_OF_BITS_TO_COUNT				256
#if (used_type_t == uint8_t)
#define NUMBER_OF_BITS_IN_ONE_VARIABLE		8
#endif
#define NUMBER_OF_VARIABLES					(NUMBER_OF_BITS_TO_COUNT/NUMBER_OF_BITS_IN_ONE_VARIABLE)

uint16_t count_ones(used_type_t values[NUMBER_OF_VARIABLES])
{
	uint16_t numberOfOnes = 0;
	uint8_t i, j;


count_ones_label0:for (i = 0; i<NUMBER_OF_VARIABLES; i++)
{

count_ones_label1:for (j = 0; j<NUMBER_OF_BITS_IN_ONE_VARIABLE; j++)
{
	//if((values[i] & (1<<j)) == (1<<j))
	// Ÿle - & ma ni¿szy priorytet ni¿ ==, co ciekawe + ma wy¿szy
	if (values[i] & (1 << j) == (1 << j))
		// Ÿle - jak wy¿ej
		//if(values[i] & (1<<j) != 0)
	{
		numberOfOnes++;
	}
}
}

				  return numberOfOnes;
}

int testCountOnes()
{
	used_type_t inputArgument[NUMBER_OF_VARIABLES] = { 2, 4, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 2, 1, 1 };

	uint16_t outputValue = count_ones(inputArgument);

	int retval;

	if (outputValue == 16)
	{
		retval = 0;
		printf("Test passed !\n");
	}
	else
	{
		retval = 1;
		printf("Test failed  !!!\n");
	}

	// Return 0 if the test
	return retval;
}

struct SZespolone
{
	int re;
	int im;

	friend ostream& operator<<(ostream& os, const SZespolone z);
};

ostream& operator<<(ostream& os, const SZespolone z)
{
	os << z.re << ' ' << z.im << '\n';
	return os;
}

static inline void TestOstreamOverloadedOperator(void)
{
	SZespolone z;
	z.re = 5;
	z.im = 3;

	std::cout << z;
}


static inline void ObjectRecognition(void)
{
	Mat imgScene = imread("rozpoznawanie//scena1.png", IMREAD_GRAYSCALE);

	const char windowName[] = "Good Matches & Object detection";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	int surfThreshold = 400;
	double minimumDistanceMultiplayer = 2.0;

	char filename[50];
	for (int j = 0; j<4; ++j)
	{
		sprintf(filename, "rozpoznawanie//obiekt%d.png", j + 1);

		Mat imgObject = imread(filename, IMREAD_GRAYSCALE);

		//-- Step 1: Detect the keypoints using SURF Detector
		SurfFeatureDetector detector(surfThreshold);

		std::vector<KeyPoint> keypointsObject, keypointsScene;

		detector.detect(imgObject, keypointsObject);
		detector.detect(imgScene, keypointsScene);

		//-- Step 2: Calculate descriptors (feature vectors)
		SurfDescriptorExtractor extractor;

		Mat descriptorsObject, descriptorsScene;

		extractor.compute(imgObject, keypointsObject, descriptorsObject);
		extractor.compute(imgScene, keypointsScene, descriptorsScene);

		//-- Step 3: Matching descriptor vectors using FLANN matcher
		//FlannBasedMatcher matcher;
		BFMatcher matcher(NORM_L2, false);
		std::vector< DMatch > matches;
		matcher.match(descriptorsObject, descriptorsScene, matches);

		double maxDistance = 0; double minDistance = 100;

		//-- Quick calculation of max and min distances between keypoints
		for (int i = 0; i < descriptorsObject.rows; ++i)
		{
			double distance = matches[i].distance;
			if (distance < minDistance)
				minDistance = distance;
			if (distance > maxDistance)
				maxDistance = distance;
		}

		printf("-- Max dist : %f \n", maxDistance);
		printf("-- Min dist : %f \n", minDistance);

		//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
		std::vector< DMatch > goodMatches;

		for (int i = 0; i < descriptorsObject.rows; ++i)
		{
			if (matches[i].distance < minimumDistanceMultiplayer*minDistance)
			{
				goodMatches.push_back(matches[i]);
			}
		}

		Mat imgMatches;
		drawMatches(imgObject, keypointsObject, imgScene, keypointsScene, goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//-- Localize the object
		std::vector<Point2f> obj;
		std::vector<Point2f> scene;

		for (int i = 0; i < goodMatches.size(); ++i)
		{
			//-- Get the keypoints from the good matches
			obj.push_back(keypointsObject[goodMatches[i].queryIdx].pt);
			scene.push_back(keypointsScene[goodMatches[i].trainIdx].pt);
		}

		Mat H = findHomography(obj, scene, RANSAC);

		//-- Get the corners from the image_1 ( the object to be "detected" )
		std::vector<Point2f> objCorners(4);
		objCorners[0] = cvPoint(0, 0);
		objCorners[1] = cvPoint(imgObject.cols, 0);
		objCorners[2] = cvPoint(imgObject.cols, imgObject.rows);
		objCorners[3] = cvPoint(0, imgObject.rows);

		std::vector<Point2f> sceneCorners(4);

		perspectiveTransform(objCorners, sceneCorners, H);

		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		circle(imgMatches, sceneCorners[0] + Point2f(imgObject.cols, 0), 5, Scalar(0, 255, 0), 4);
		circle(imgMatches, sceneCorners[1] + Point2f(imgObject.cols, 0), 5, Scalar(255, 0, 0), 4);
		circle(imgMatches, sceneCorners[2] + Point2f(imgObject.cols, 0), 5, Scalar(0, 0, 255), 4);
		circle(imgMatches, sceneCorners[3] + Point2f(imgObject.cols, 0), 5, Scalar(0, 255, 255), 4);
		line(imgMatches, sceneCorners[0] + Point2f(imgObject.cols, 0), sceneCorners[1] + Point2f(imgObject.cols, 0), Scalar(0, 255, 0), 4);
		line(imgMatches, sceneCorners[1] + Point2f(imgObject.cols, 0), sceneCorners[2] + Point2f(imgObject.cols, 0), Scalar(0, 255, 0), 4);
		line(imgMatches, sceneCorners[2] + Point2f(imgObject.cols, 0), sceneCorners[3] + Point2f(imgObject.cols, 0), Scalar(0, 255, 0), 4);
		line(imgMatches, sceneCorners[3] + Point2f(imgObject.cols, 0), sceneCorners[0] + Point2f(imgObject.cols, 0), Scalar(0, 255, 0), 4);

		//-- Show detected matches
		imshow(windowName, imgMatches);

		//imwrite("DDD_do_prezentacji.png", imgMatches);

		waitKey(0);
	}
}

void loadMovie()
{
	//VideoCapture inputVideo("F:\\Amin\\Desktop\\00007.MTS");
	VideoCapture inputVideo("F:\\Amin\\Desktop\\dla_studentow_nowa.wmv");
	//VideoCapture inputVideo("F:\\Amin\\Desktop\\00008.mp4");
	//VideoCapture inputVideo("F:\\Amin\\Desktop\\dt_passat.mpg");

	Mat currentFrame;
	inputVideo >> currentFrame;

	const char windowName[] = "Main";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	cout << "Start reading input video file\r\n";

	for (;;)
	{
		cout << inputVideo.grab() << ", ";
		inputVideo.retrieve(currentFrame);

		//capture>>frame;

		imshow(windowName, currentFrame);

		waitKey(20);
	}


}

int main(int, char**)
{
	ObjectRecognition();

	loadMovie();

	myObjectDetect();

	PSEyeTest();

	TestOstreamOverloadedOperator();
	//

	//testCountOnes();

	//loadPGMfile();
	//testQueueStack();
	//dynamicTableTest();

	//myObjectDetect();

	//ObjectTracking();

	int a = 2["01"];
	int b[] = { 1, 2, 3, 4 };
	cout << 2[b];
	cout << 2["1234567890"];
	cout << *(2 + b);

	char tab1[] = "abc";
	char* tab2 = tab1;
	tab2[0] = 'A';

	int* tab = new int[5];
	delete[] tab;

	/// Create a window to display results
	char* windowName = "Main window";
	namedWindow(windowName, WINDOW_AUTOSIZE);

	// trackbar parameters
	const int maxValue = 255;
	const int  maxType = 4;

	char* trackbarType = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
	char* trackbarValue = "Value";

	/// Create Trackbar to choose type and value
	createTrackbar(trackbarType, windowName, &G_thresholdType, maxType, TrackbarFunction);
	createTrackbar(trackbarValue, windowName, &G_thresholdValue, maxValue, TrackbarFunction);

	setMouseCallback(windowName, onMouse);

	//ThresholdTest(G_thresholdValue, G_thresholdType, windowName);
	//FindMarkersAndTransform(G_thresholdValue, G_thresholdType, windowName);
	//ExerciseFindMarkersAndDoThePerspectiveTransform(G_thresholdValue, G_thresholdType, windowName);

	//AdamDance();

	//SobelTest();

	//ErodeTest();

	//MedianTest();

	/*
	StopWatch sw;

	sw.Start();

	MatricesAreFunPart1();
	system("pause");

	cout<<endl<<sw.GetElapsedTimeInMiliseconds()<<endl;

	MatricesAreFunPart2();
	system("pause");

	cout<<endl<<sw.GetElapsedTimeInMiliseconds()<<endl;

	MatricesAreFunPart3();
	system("pause");

	cout<<endl<<sw.GetElapsedTimeInMiliseconds()<<endl;

	system("pause");
	*/

	//BlendingTwoImages();

	//NegativeTest();

	//BlurTest();

	//ScanImageAndReduceTest();

	//FileInputOutputTest();

	interpolationTest();

	//testWejsciowka();

	//wczytywanie: flaga okre´sla liczb²e kana³ów obrazu
	// -1 - bez zmian, 0 - w skali szaro´sci, 1 - kolorowy
	Mat image = imread("resources//programmers_tan.jpg", -1);
	//zapisywanie
	imwrite("resources//programmers_tan_copy.jpg", image);

	//jeÿzeli jest tylko jedna kamera to moÿzna przekaza´c numer -1
	VideoCapture capture(0);	// numer_kamery
	//VideoCapture z_pliku(sciezka);

	//zmienna przechowujaca klatke
	Mat frame;
	//zatrza´sni²ecie klatki wewn ² atrz kamery bez jej odzyskania
	//szybkie, pozwala na synchronizacj²e kamer:
	capture.grab();
	//odzyskanie zatrzasnietej klatki:
	capture.retrieve(frame);
	//obie powyÿzsze operacje:
	capture.read(frame);
	//lub:
	capture >> frame;

	FileStorage fs("resources//testFS.xml", FileStorage::WRITE);
	fs << "asd" << frame;

	namedWindow("okno");
	imshow("okno", image);
	//czeka na dowolny klawisz
	waitKey(0);
	blur(image, image, Size(3, 3), Point(-1, -1));
	//wyswietla wygladzony obraz
	imshow("okno", image);
	//czeka na dowolny klawisz
	waitKey(0);
	imshow("okno", frame);
	waitKey(0);

	src = image.clone();
	trackbarValue = 150;
	namedWindow("TrackbarWindowName");
	createTrackbar("TrackbarName", "TrackbarWindowName", &trackbarValue, trackbarValueMax, on_trackbar);
	// Show some stuff
	on_trackbar(trackbarValue, 0);

	// Wait until user press some key
	waitKey(0);

	capture.release();
}




