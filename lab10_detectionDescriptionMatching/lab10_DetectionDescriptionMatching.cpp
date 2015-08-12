#include <opencv2/opencv.hpp>

using namespace cv;

#include <vector>

int main(void)
{
	// does not work with my compilation of opencv
	//SurfFeatureDetector myFeatureDetector = new SurfFeatureDetector(100);

	std::vector<cv::String> listOfParameters;

	const char* windowName_img1WithKeypoints = "img1Keypoints";
	const char* windowName_img2WithKeypoints = "img2Keypoints";
	const char* windowName_imgMatches = "imgMatches";

	namedWindow(windowName_img1WithKeypoints);
	namedWindow(windowName_img2WithKeypoints);
	namedWindow(windowName_imgMatches);

	Mat imgScene1;
	Mat imgScene2;

	imgScene1 = imread("resources/forward-1.bmp", IMREAD_COLOR);
	imgScene2 = imread("resources/forward-2.bmp", IMREAD_COLOR);

	const double scale = 0.5;

	//resize(imread("resources/Basler_acA1600-60gc__21433617__20150216_155054805_0128.png", IMREAD_COLOR), imgScene1, cv::Size(), 0.5, 0.5);
	//resize(imread("resources/Basler_acA1600-60gc__21433617__20141019_175350098_0387.jpg", IMREAD_COLOR), imgScene1, cv::Size(), scale, scale);

	//resize(imread("resources/Basler_acA1600-60gc__21433617__20150216_155054805_0136.png", IMREAD_COLOR), imgScene2, cv::Size(), 0.5, 0.5);
	//resize(imread("resources/Basler_acA1600-60gc__21433617__20141019_175350098_0397.jpg", IMREAD_COLOR), imgScene2, cv::Size(), scale, scale);

	std::vector<KeyPoint> keypointsFromImage1;
	std::vector<KeyPoint> keypointsFromImage2;

	Ptr<FeatureDetector> det = FeatureDetector::create("FAST");
	det->getParams(listOfParameters);
	std::cout << "Detector available params: " << std::endl;
	for (auto param : listOfParameters)
	{
		std::cout << param << std::endl;
	}

	std::cout << "Threshold before change: " << det->get<int>("threshold") << std::endl;
	det->set("threshold", 90);
	std::cout << "Threshold after change: " << det->get<int>("threshold") << std::endl;

	det->detect(imgScene1, keypointsFromImage1);
	det->detect(imgScene2, keypointsFromImage2);

	Mat imgScene1WithKeypoints;
	Mat imgScene2WithKeypoints;
	drawKeypoints(imgScene1, keypointsFromImage1, imgScene1WithKeypoints);
	drawKeypoints(imgScene2, keypointsFromImage2, imgScene2WithKeypoints);

	imshow(windowName_img1WithKeypoints, imgScene1WithKeypoints);
	imshow(windowName_img2WithKeypoints, imgScene2WithKeypoints);

	Ptr<DescriptorExtractor> desc = DescriptorExtractor::create("ORB");
	desc->getParams(listOfParameters);
	std::cout << "Descriptor available params: " << std::endl;
	for (auto param : listOfParameters)
	{
		std::cout << param << std::endl;
	}

	Mat descriptorsFromImage1;
	Mat descriptorsFromImage2;
	desc->compute(imgScene1, keypointsFromImage1, descriptorsFromImage1);
	desc->compute(imgScene2, keypointsFromImage2, descriptorsFromImage2);

	Ptr<DescriptorMatcher> match = DescriptorMatcher::create("BruteForce");
	match->getParams(listOfParameters);
	std::cout << "Matcher available params: " << std::endl;
	for (auto param : listOfParameters)
	{
		std::cout << param << std::endl;
	}
	std::cout << "normType before change: " << match->get<int>("normType") << std::endl;
	match->set("normType", NORM_HAMMING);
	std::cout << "normType after change: " << match->get<int>("normType") << std::endl;

	/*
	// norm types
	enum {
	NORM_INF = 1,
	NORM_L1 = 2,
	NORM_L2 = 4,
	NORM_L2SQR = 5,
	NORM_HAMMING = 6,
	NORM_HAMMING2 = 7,
	NORM_TYPE_MASK = 7,
	NORM_RELATIVE = 8,
	NORM_MINMAX = 32
	};
	*/
	std::vector<DMatch> matches;
	match->match(descriptorsFromImage1, descriptorsFromImage2, matches);

	for (auto itr = matches.begin(); itr != matches.end();)
	{
		if ((*itr).distance < 10.0)
		{
			//itr = matches.erase(itr);
			itr++;
		}
		else
		{
			itr++;
		}
	}

	Mat imageWithMatches;
	drawMatches(imgScene1, keypointsFromImage1, imgScene2, keypointsFromImage2, matches, imageWithMatches);

	imshow(windowName_imgMatches, imageWithMatches);

	waitKey(0);

	return 0;
}