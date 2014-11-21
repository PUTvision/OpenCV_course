#include "opencv2/opencv.hpp"

using namespace cv;

#include <cstdint>		// for uint8_t type



#include <iostream>

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

int main(int, char**)
{
	//testWejsciowka();

	//Mat matrix;

	//cout << endl << "Single channel matrix:" << endl;
	//matrix.create(4, 4, CV_8UC1);
	//cout << "matrix.create(4, 4, CV_8UC1);" << endl << matrix << endl;
	//matrix.setTo(Scalar(128));
	//cout << "matrix.setTo(Scalar(128));" << endl << matrix << endl;

	//cout << endl << "Multi-channel matrix:" << endl;
	//matrix.create(2, 2, CV_8UC3);
	//cout << "matrix.create(3, 3, CV_8UC3);" << endl << matrix << endl;
	//matrix.setTo(Scalar(1, 2, 3, 4));
	//cout << "matrix.setTo(Scalar(1, 2, 3, 4));" << endl << matrix << endl;

	//cout << endl << "Matlab style:" << endl;
	//matrix = Mat::eye(5, 5, CV_64FC1);
	//cout << "matrix = Mat::eye(5, 5, CV_64FC1);" << endl << matrix << endl;

	//cout << endl;
	//matrix = (Mat_<double>(3, 3) << 11, 12, 13, 21, 22, 23, 31, 32, 33);
	//cout << "Cala macierz:" << endl << matrix << endl;
	//cout << "Wyswietlanie tylko fragmentu macierzy:" << endl;
	//cout << "Pierwszy wiersz: matrix.row(0):" << matrix.row(0) << endl;
	//cout << "Ostatnia kolumna: matrix.col(matrix.cols-1):" << matrix.col(matrix.cols - 1) << endl;

	//cout << endl;

	//cout << endl;
	//Mat mat1, mat2;
	//mat1.create(Size(5, 5), CV_8UC1);
	//randu(mat1, Scalar::all(0), Scalar::all(10));
	//Mat mat3(mat1, Rect(2, 2, 2, 2));
	//mat2 = mat1;
	//cout << mat1 << endl << mat2 << endl << mat3 << endl;
	//mat2.setTo(6);
	//cout << mat1 << endl << mat2 << endl << mat3 << endl;
}




