#include "opencv2/opencv.hpp"

// Ceemple OpenCV 3.0
// based on OpenCV example: http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/feature_homography/feature_homography.html#feature-homography

using namespace cv;

#include <vector>

void ObjectRecognition(void)
{
	Mat imgScene = imread("..\\data\\object_recognition\\scena1.png", IMREAD_GRAYSCALE);

	const char windowName[] = "Good Matches & Object detection";
	namedWindow(windowName, WINDOW_AUTOSIZE | WINDOW_KEEPRATIO);

	double minimumDistanceMultiplayer = 2.5;

	char filename[50];
	for (int j = 0; j<4; ++j)
	{
		sprintf(filename, "..\\data\\object_recognition\\obiekt%d.png", j + 1);

		Mat imgObject = imread(filename, IMREAD_GRAYSCALE);

		//-- Step 1: Detect the keypoints using detector

		// SURF - not available in Ceemple
		//int surf_threshold = 400;
		//SurfFeatureDetector detector(surf_threshold);
		// SurfDescriptorExtractor extractor;
		
		// ORB - binary detector and descriptor
		Ptr<ORB> detector_and_descriptor = ORB::create();

		std::vector<KeyPoint> keypointsObject, keypointsScene;

		detector_and_descriptor->detect(imgObject, keypointsObject);
		detector_and_descriptor->detect(imgScene, keypointsScene);

		//-- Step 2: Calculate descriptors (feature vectors)
		Mat descriptorsObject, descriptorsScene;

		// SURF version
		//extractor.compute(imgObject, keypointsObject, descriptorsObject);
		//extractor.compute(imgScene, keypointsScene, descriptorsScene);

		// ORB version
		detector_and_descriptor->compute(imgObject, keypointsObject, descriptorsObject);
		detector_and_descriptor->compute(imgScene, keypointsScene, descriptorsScene);

		//-- Step 3: Matching descriptor vectors using matcher
		std::vector< DMatch > matches;

		// SURF version
		//BFMatcher matcher(NORM_L2);

		// ORB version
		BFMatcher matcher(NORM_HAMMING);

		matcher.match(descriptorsObject, descriptorsScene, matches);

		//-- Step 4: Parse points based on distance
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

		//-- Draw only "good" matches
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

		//-- Find the transofrmation between object and secene
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