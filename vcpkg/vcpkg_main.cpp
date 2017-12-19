#include <opencv2/opencv.hpp>
#include <iostream>

static void onMouse(int event, int x, int y, int, void*)
{
	if (event == cv::EVENT_LBUTTONDOWN)
		std::cout << "EVENT_LBUTTONDOWN" << std::endl;
	if (event == cv::EVENT_LBUTTONUP)
		std::cout << "EVENT_LBUTTONUP" << std::endl;
	if (event == cv::EVENT_RBUTTONDOWN)
		std::cout << "EVENT_RBUTTONDOWN" << std::endl;
	if (event == cv::EVENT_RBUTTONUP)
		std::cout << "EVENT_RBUTTONUP" << std::endl;
}

int main()
{
	cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);

	cv::setMouseCallback("raw", onMouse, 0);

	cv::VideoCapture cap;
	//cap.open(0);
	cap.open("Wildlife.mp4");
	//cap.open("F:/Amin/Desktop/SISW projekt/00000.MTS");
	//cap.open("F:\\Amin\\Desktop\\SISW projekt\\0000.MTS");

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return -1;
	}

	cv::UMat bgr_frame, gray, canny;

	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;

		cv::imshow("raw", bgr_frame);

		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", gray);

		cv::Canny(gray, canny, 10, 100, 3, true);
		cv::imshow("canny", canny);

		char c = cv::waitKey(10);
		if (c == 27) break;
	}

	cap.release();
	return 0;
}