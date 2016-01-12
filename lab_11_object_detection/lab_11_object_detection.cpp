#include "opencv2/opencv.hpp"

using namespace cv;

#include <vector>

void ObjectRecognition(void)
{
	Mat imgScene = imread("..\\data\\object_recognition\\scena1.png", IMREAD_GRAYSCALE);

	const char windowName[] = "Good Matches & Object detection";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	int surfThreshold = 400;
	double minimumDistanceMultiplayer = 2.5;

	char filename[50];
	for (int j = 0; j<4; ++j)
	{
		//sprintf(filename, "rozpoznawanie//obiekt%d.png", j + 1);
		sprintf(filename, "..\\data\\object_recognition\\obiekt%d.png", j + 1);

		Mat imgObject = imread(filename, IMREAD_GRAYSCALE);

		//-- Step 1: Detect the keypoints using SURF Detector
		//SurfFeatureDetector detector(surfThreshold);
		//Ptr<FeatureDetector> detector_and_descriptor = FeatureDetector::create("ORB");

		//Ptr<AKAZE> detector_and_descriptor = AKAZE::create();
		Ptr<ORB> detector_and_descriptor = ORB::create();

		std::vector<KeyPoint> keypointsObject, keypointsScene;

		//detector.detect(imgObject, keypointsObject);
		//detector.detect(imgScene, keypointsScene);
		detector_and_descriptor->detect(imgObject, keypointsObject);
		detector_and_descriptor->detect(imgScene, keypointsScene);

		//-- Step 2: Calculate descriptors (feature vectors)
		//SurfDescriptorExtractor extractor;

		Mat descriptorsObject, descriptorsScene;

		//extractor.compute(imgObject, keypointsObject, descriptorsObject);
		//extractor.compute(imgScene, keypointsScene, descriptorsScene);
		detector_and_descriptor->compute(imgObject, keypointsObject, descriptorsObject);
		detector_and_descriptor->compute(imgScene, keypointsScene, descriptorsScene);

		//-- Step 3: Matching descriptor vectors using FLANN matcher
		//FlannBasedMatcher matcher;
		//BFMatcher matcher(NORM_L2, false);
		BFMatcher matcher(NORM_HAMMING, false);
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
		drawMatches(imgObject, keypointsObject, imgScene, keypointsScene, goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

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

int main(int, char)
{
	ObjectRecognition();

	return 0;
}