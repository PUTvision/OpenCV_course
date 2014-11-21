#include <opencv2\core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2\highgui.hpp>		// OpenCV window I/O
#include <opencv2\calib3d.hpp>		// camera calibration
#include <opencv2\imgproc.hpp>		// cornerSubPix function

using namespace cv;

#include <iostream>
#include <stdio.h>
#include <vector>

int main(void)
{
	// window preparation
	const char* WINDOW_chessboardCorners = "Chessboard corners";
	const char* WINDOW_originalImage = "Original image";
	const char* WINDOW_calibrationImage = "Calibrated image";
	
	// storage for loaded images
	const int numbeOfImages = 15;
	std::vector<Mat> images;

	// process of loading the images (important to load them as greyscale!)
	for (int i = 0; i<numbeOfImages; ++i)
	{
		char filename[40];
		//Mat imgFromFile = imread("/../../resources/calibration/img_21130751_0000.bmp", IMREAD_GRAYSCALE);
		sprintf(filename, "resources/calibration/img_21130751_%04d.bmp", i);
		std::cout << filename << std::endl;
		images.push_back(imread(filename, IMREAD_GRAYSCALE));
	}

	// parameters for chessboad pattern and finding corners
	Size patternsize(8, 5); //interior number of corners
	TermCriteria term(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1);
	// object for holding the corners coordinates in all the images
	std::vector<std::vector<Point2f>> cornersFromAllImages;

	for (int i = 0; i < images.size(); i++)
	{
		std::vector<Point2f> corners; //this will be filled by the detected corners

		//CALIB_CB_FAST_CHECK saves a lot of time on images that do not contain any chessboard corners
		bool patternfound = findChessboardCorners(images[i], patternsize, corners,
			CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);

		if (patternfound)
		{
			cornerSubPix(images[i], corners, Size(11, 11), Size(-1, -1), term);
			cornersFromAllImages.push_back(corners);
		}
	}

	// the snippet below shows how to display the corners (works only if the corners were found on first image
	Mat imgToShow = images[0].clone();
	drawChessboardCorners(imgToShow, patternsize, Mat(cornersFromAllImages[0]), true);
	imshow(WINDOW_chessboardCorners, imgToShow);
	waitKey();

	// create the model corners
	std::vector<std::vector<Point3f>> cornersModelForAllImages;
	for (int i = 0; i < cornersFromAllImages.size(); ++i)
	{
		std::vector<Point3f> cornersModel;
		for (int j = 0; j < patternsize.height; ++j)
		{
			for (int k = 0; k < patternsize.width; ++k)
			{
				cornersModel.push_back(Point3f(k, j, 0.0F));
			}
		}
		cornersModelForAllImages.push_back(cornersModel);
	}

	Mat cameraMatrix, distCoeffs;
	std::vector<Mat> rvec, tvecs;

	double rms = calibrateCamera(cornersModelForAllImages, cornersFromAllImages, images[0].size(), cameraMatrix, distCoeffs, rvec, tvecs);

	std::cout << cameraMatrix << std::endl;
	std::cout << distCoeffs << std::endl;
	std::cout << rms << std::endl;

	Mat undistortedImage;

	undistort(images[0], undistortedImage, cameraMatrix, distCoeffs);

	imshow(WINDOW_originalImage, images[0]);
	imshow(WINDOW_calibrationImage, undistortedImage);
	waitKey();

	return 0;

}

