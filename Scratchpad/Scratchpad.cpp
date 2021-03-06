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

void Kuwahara_test()
{
	Mat img = imread("..\\data\\kuwahara\\original.png");
	Mat average, variance;
	average.create(img.rows, img.cols, CV_32SC1);
	variance.create(img.rows, img.cols, CV_32SC1);

	if (img.isContinuous())
	{
		int rows = img.rows;
		int cols = img.cols;
		int step = img.step;

		unsigned char *pImg = (unsigned char*)(img.data);

		for (int i = 1; i < rows - 1; ++i)
		{
			for (int j = 1; j < cols - 1; ++j)
			{
				pImg[step*i + j] = 555;
				pImg[step*i + j + 1] = 555;
				pImg[step*i + j + 2] = 555;
			}
		}
	}
	else
	{
		CV_Error(-666, "Mat is not continous");
	}

	// this checks if the image is continuous and works with color images
	if (img.isContinuous())
	{
		int numberOfChannels = img.channels();
		int numberOfPixels = img.rows * img.cols * numberOfChannels;
		uchar* p = img.data;
		for (int i = 0; i < numberOfPixels; i = i + numberOfChannels)
		{



			for (int j = 0; j < numberOfChannels; ++j)
			{
				p[i + j] = 255 - p[i + j];
			}
		}
	}

}

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
	// czemu nie da si� skompilowa� decolor?
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

static void ShowValues(int &intVal, double &doubleVal, std::string &stringVal, Mat &mat)
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

static void loadPGMfile(void)
{
	Mat image = imread("C://obrazy//sekwencja//p_0020.pgm", IMREAD_UNCHANGED);

	const char windowName[] = "Image from file";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	imshow(windowName, image);
	waitKey(0);
}

int main(int, char**)
{
	PSEyeTest();

	//loadPGMfile();

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

	//testWejsciowka();

	//wczytywanie: flaga okre�sla liczb�e kana��w obrazu
	// -1 - bez zmian, 0 - w skali szaro�sci, 1 - kolorowy
	Mat image = imread("resources//programmers_tan.jpg", -1);
	//zapisywanie
	imwrite("resources//programmers_tan_copy.jpg", image);

	//je�zeli jest tylko jedna kamera to mo�zna przekaza�c numer -1
	VideoCapture capture(0);	// numer_kamery
	//VideoCapture z_pliku(sciezka);

	//zmienna przechowujaca klatke
	Mat frame;

	capture.grab();
	//odzyskanie zatrzasnietej klatki:
	capture.retrieve(frame);
	//obie powy�zsze operacje:
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




