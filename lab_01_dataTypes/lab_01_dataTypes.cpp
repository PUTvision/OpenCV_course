#include "opencv2/opencv.hpp"

// Using CeempleCVand OpenCV 3.X

using namespace cv;

void MatricesAreFunPart1(void)
{
	Mat matrix;

	std::cout << std::endl << "Single channel matrix:" << std::endl;
	matrix.create(4, 4, CV_8UC1);
	std::cout << "matrix.create(4, 4, CV_8UC1);" << std::endl << matrix << std::endl;
	matrix.setTo(Scalar(128));
	std::cout << "matrix.setTo(Scalar(128));" << std::endl << matrix << std::endl;

	std::cout << std::endl << "Multi-channel matrix:" << std::endl;
	matrix.create(2, 2, CV_8UC3);
	std::cout << "matrix.create(3, 3, CV_8UC3);" << std::endl << matrix << std::endl;
	matrix.setTo(Scalar(1, 2, 3, 4));
	std::cout << "matrix.setTo(Scalar(1, 2, 3, 4));" << std::endl << matrix << std::endl;

	std::cout << std::endl << "Matlab style:" << std::endl;
	matrix = Mat::eye(5, 5, CV_64FC1);
	std::cout << "matrix = Mat::eye(5, 5, CV_64FC1);" << std::endl << matrix << std::endl;

	std::cout << std::endl;
	matrix = (Mat_<double>(3, 3) << 11, 12, 13, 21, 22, 23, 31, 32, 33);
	std::cout << "Cala macierz:" << std::endl << matrix << std::endl;
	std::cout << "Wyswietlanie tylko fragmentu macierzy:" << std::endl;
	std::cout << "Pierwszy wiersz: matrix.row(0):" << matrix.row(0) << std::endl;
	std::cout << "Ostatnia kolumna: matrix.col(matrix.cols-1):" << matrix.col(matrix.cols - 1) << std::endl;

	std::cout << std::endl;
}

void MatricesAreFunPart2(void)
{
	Mat image = imread("..\\data\\lena.jpg", IMREAD_GRAYSCALE);
	std::cout << std::endl << "Wczytany obraz piksel (0,  0):" << image.row(0).col(0) << std::endl;
}

void MatricesAreFunPart3(void)
{
	std::cout << std::endl;
	Mat mat1, mat2;
	mat1.create(Size(5, 5), CV_8UC1);
	randu(mat1, Scalar::all(0), Scalar::all(10));
	Mat mat3(mat1, Rect(2, 2, 2, 2));
	mat2 = mat1;
	std::cout << mat1 << std::endl << mat2 << std::endl << mat3 << std::endl;
	mat2.setTo(6);
	std::cout << mat1 << std::endl << mat2 << std::endl << mat3 << std::endl;
}


int main(int, char)
{
	// TODO: check if this works
	MatricesAreFunPart1();
	MatricesAreFunPart2();
	MatricesAreFunPart3();

	return 0;
}