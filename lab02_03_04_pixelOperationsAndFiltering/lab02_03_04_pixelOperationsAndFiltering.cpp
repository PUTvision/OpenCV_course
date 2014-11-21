#include "opencv2/opencv.hpp"

using namespace cv;

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

void NegativeDirectMemoryAccessTuned4Bytes(Mat& img)
{
	int numberOfElemnts = img.rows * img.cols / sizeof(int64);
	int64* p = (int64*)img.data;
	for (int j = 0; j < numberOfElemnts; ++j)
	{
		p[j] = ~p[j];
	}
}

void NegativeDirectMemoryAccessTunedEvenMore(Mat& img)
{
	/*
	int numberOfElemnts = img.rows * img.cols / sizeof(int);
	int* p = (int*)img.data;
	for (int j = 0; j < numberOfElemnts; j+=8)
	{
	p[j] = ~p[j];
	p[j+1] = ~p[j+1];
	p[j+2] = ~p[j+2];
	p[j+3] = ~p[j+3];
	p[j+4] = ~p[j+4];
	p[j+5] = ~p[j+5];
	p[j+6] = ~p[j+6];
	p[j+7] = ~p[j+7];
	}
	*/


	int step1 = img.rows * img.cols;
	uchar* src1 = img.data;
	uchar* dst1 = img.data;


	int i = 0;
	for (; i < step1; i += 16)
	{
		const int* src1i = (const int*)(src1 + i);
		int* dst1i = (int*)(dst1 + i);
		int t0 = ~(src1i)[0];
		int t1 = ~(src1i)[1];
		int t2 = ~(src1i)[2];
		int t3 = ~(src1i)[3];

		dst1i[0] = t0;
		dst1i[1] = t1;
		dst1i[2] = t2;
		dst1i[3] = t3;
	}

}

void NegativeDirectMemoryAccessUniversal(Mat& img)
{
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
	else
	{
		CV_Error(-666, "Mat is not continous");
	}
}

void NegativeIteratorUniversal(Mat& img)
{
	// this functions works with color images
	int numberOfChannels = img.channels();

	if (numberOfChannels == 1)
	{
		MatIterator_<uchar> it, end;
		for (it = img.begin<uchar>(), end = img.end<uchar>(); it != end; ++it)
		{
			*it = 255 - *it;
		}
	}
	else if (numberOfChannels == 3)
	{
		MatIterator_<Vec<uchar, 3>> it, end;
		for (it = img.begin<Vec<uchar, 3>>(), end = img.end<Vec<uchar, 3>>(); it != end; ++it)
		{
			// there are two approaches to get Vec values:
			// through val filed
			(*it).val[0] = 255 - (*it).val[0];
			// or directly
			(*it)[1] = 255 - (*it)[1];

			(*it).val[2] = 255 - (*it).val[2];
		}
	}
	else
	{
		CV_Error(-666, "Unsuported number of channels");
	}
}

void NegativeRandomAccessBGR(Mat& img)
{
	// this function works only for color images
	typedef Vec<uchar, 3> Pixel;

	int rows = img.rows;
	int cols = img.cols;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			img.at<Pixel>(i, j).val[0] = 255 - img.at<Pixel>(i, j).val[0];
			img.at<Pixel>(i, j).val[1] = 255 - img.at<Pixel>(i, j).val[1];
			img.at<Pixel>(i, j).val[2] = 255 - img.at<Pixel>(i, j).val[2];
		}
	}
}

void NegativRawPtrAccessBGR(Mat& img)
{
	// this function works only for color images
	typedef Vec<uchar, 3> Pixel;

	for (int r = 0; r < img.rows; ++r)
	{
		Pixel* ptr = img.ptr<Pixel>(r, 0);
		const Pixel* ptr_end = ptr + img.cols;
		for (; ptr != ptr_end; ++ptr)
		{
			ptr->val[0] = 255 - ptr->val[0];
			ptr->val[1] = 255 - ptr->val[1];
			ptr->val[2] = 255 - ptr->val[2];
		}
	}
}

void NegativeMatIteratorBGR(Mat& img)
{
	// this function works only for color images
	typedef Vec<uchar, 3> Pixel;

	for (Pixel &p : cv::Mat_<Pixel>(img))
	{
		p.val[0] = 255 - p.val[0];
		p.val[1] = 255 - p.val[1];
		p.val[2] = 255 - p.val[2];
	}
}

void NegativeLambdaRGB(Mat& img)
{
	// this function works only for color images
	typedef Vec<uchar, 3> Pixel;

	// using C++11 lambda.
	img.forEach<Pixel>([](Pixel &p, const int * position) -> void {
		p.val[0] = 255 - p.val[0];
		p.val[1] = 255 - p.val[1];
		p.val[2] = 255 - p.val[2];
	});
}

void NegativeRowColAccess(Mat& img)
{
	int rows = img.rows;
	int cols = img.cols;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			img.row(i).col(j) = ~img.row(i).col(j);
		}
	}
}

void NegativeDirectMemoryAccessGrayscale(Mat& img)
{
	// this checks if the image is continuous but assumes that the image is grayscale
	if (img.isContinuous())
	{
		int numberOfPixels = img.rows * img.cols;
		uchar* p = img.data;
		for (int i = 0; i < numberOfPixels; ++i)
		{
			p[i] = 255 - p[i];
		}
	}
	else
	{
		CV_Error(-666, "Mat is not continous");
	}
}

void NegativeIteratorSimpleGrayscale(Mat& img)
{
	// this function assumes that the image is grayscale
	MatIterator_<uchar> it, end;
	for (it = img.begin<uchar>(), end = img.end<uchar>(); it != end; ++it)
	{
		*it = 255 - *it;
	}
}

void NegativeRandomAccessGrayscale(Mat& img)
{
	int rows = img.rows;
	int cols = img.cols;

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			img.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);
		}
	}
}

void NegativRawPtrAccessGreyscale(Mat& img)
{
	for (int r = 0; r < img.rows; ++r)
	{
		unsigned char* ptr = img.ptr<unsigned char>(r, 0);
		const unsigned char* ptr_end = ptr + img.cols;
		for (; ptr != ptr_end; ++ptr)
		{
			*ptr = 255 - *ptr;
		}
	}
}

void NegativeMatIteratorGrayscale(Mat& img)
{
	for (unsigned char &p : cv::Mat_<unsigned char>(img))
	{
		p = 255 - p;
	}
}

void NegativeLambdaGrayscale(Mat& img)
{
	img.forEach<unsigned char>([](unsigned char &p, const int * position) -> void {
		p = 255 - p;
	});
}

/*
static inline void NegativeTest(void)
{
Mat src = imread("opalenizna programisty.jpg", IMREAD_GRAYSCALE);
Mat dst = src.clone();
const int numberOfPrepareIterations = 100;
const int numberOfIterations = 1000;

MF_StopWatch sw;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
NegativeDirectMemoryAccess(dst.clone());
}
std::cout << std::endl << "DirectMemoryAccess: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
NegativeDirectMemoryAccessTuned(dst.clone());
}
std::cout << std::endl << "DirectMemoryAccessTuned: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
NegativeDirectMemoryAccessTuned4Bytes(dst.clone());
}
std::cout << std::endl << "DirectMemoryAccessTuned4Bytes: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
NegativeDirectMemoryAccessTunedEvenMore(dst.clone());
}
std::cout << std::endl << "NegativeDirectMemoryAccessTunedEvenMore: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
NegativeIterator(dst.clone());
}
std::cout << std::endl << "Iterator: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
NegativeRandomAccess(dst.clone());
}
std::cout << std::endl << "RandomAccess: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations / 100; i++)
{
NegativeRowsColsAccess(dst.clone());
}
std::cout << std::endl << "RowsColsAccess: " << sw.GetElapsedTimeInMiliseconds() * 100 << std::endl;

sw.Start();
for (int i = 0; i<numberOfIterations; i++)
{
dst = ~dst;
}
std::cout << std::endl << "Built-in negative: " << sw.GetElapsedTimeInMiliseconds() << std::endl;

namedWindow("Original", 1);
namedWindow("Negative", 1);
imshow("Original", src);
imshow("Negative", dst);

waitKey(0);
}
*/

int main(void)
{
	VideoCapture cap(0);	// open the default camera
	if (!cap.isOpened())	// check if we succeeded
	{
		return -1;
	}

	for (;;)
	{
		Mat frame;
		cap >> frame;		// get a new frame from camera
		Mat grayscale;
		cvtColor(frame, grayscale, COLOR_BGR2GRAY);

		//Mat negativeImage = frame.clone();
		Mat negativeImage = grayscale.clone();

		NegativeDirectMemoryAccessGrayscale(negativeImage);
		//NegativeIteratorSimpleGrayscale(negativeImage);
		//NegativeRandomAccessGrayscale(negativeImage);
		//NegativRawPtrAccessGreyscale(negativeImage);
		//NegativeMatIteratorGrayscale(negativeImage);
		//NegativeLambdaGrayscale(negativeImage);

		//NegativeDirectMemoryAccessUniversal(negativeImage);
		//NegativeIteratorUniversal(negativeImage);
		//NegativeRandomAccessBGR(negativeImage);
		//NegativRawPtrAccessBGR(negativeImage);
		//NegativeMatIteratorBGR(negativeImage);
		//NegativeLambdaRGB(negativeImage);

		imshow("image", negativeImage);
		if (waitKey(30) >= 0)
		{
			break;
		}
	}
	return 0;
}