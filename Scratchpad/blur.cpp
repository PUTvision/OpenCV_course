#include "blur.h"

//#define USE_OPENMP

//#define ROWS 288
//#define COLS 500


#include <iostream>
using namespace std;

/*
Integer division by a constant
When you divide an integer (that is known to be positive or zero) by a constant, convert the integer to unsigned.

If s is a signed integer, u is an unsigned integer, and C is a constant integer expression (positive or negative), the operation s / C is slower than u / C, and s % C is slower than u % C. This is most significant when C is a power of two, but in all cases, the sign must be taken into account during division.

The conversion from signed to unsigned, however, is free of charge, as it is only a reinterpretation of the same bits. Therefore, if s is a signed integer that you know to be positive or zero, you can speed up its division using the following (equivalent) expressions: (unsigned)s / C and (unsigned)s % C.
*/

//uchar tempTable[500*288];

void BlurSuperFastCorreceted(Mat& imgSource, Mat& imgResult)
{
	/*
	if(!imgSource.isContinuous() || !imgResult.isContinuous())
	{
		cout<<"images are not continous"<<endl;
	}
	*/

	const int rows = imgSource.rows;
	const int cols = imgSource.cols;

	uchar* tempTable = new uchar[rows*cols];

	int i, j;

	uchar* pSrc = imgSource.data;
	uchar* pDst = tempTable + 1;//imgResult.data + 1;
	// vertical pass
	for(i=0; i<rows; ++i)
	{
		for(j=1; j<cols-1; ++j)
		{
			*pDst = (uchar)(((unsigned int)(*pSrc + *(pSrc+1) + *(pSrc+2)))/9);

			pDst++;
			pSrc++;
		}
		pDst += 2;
		pSrc += 2;
	}

	pSrc = tempTable + cols + 1;
	pDst = imgResult.data + cols + 1;
	// horizontal pass
	for(i=1; i<rows-1; ++i)
	{
		for(j=1; j<cols-1; ++j)
		{
			*pDst = (uchar) ( ( (unsigned int) (*(pSrc-cols) + *pSrc + *(pSrc+cols)) ) );

			pDst++;
			pSrc++;
		}
		pDst += 2;
		pSrc += 2;
	}

	delete [] tempTable;
}

//uchar tempTable[500];

void BlurSuperFastCorreceted2(Mat& imgSource, Mat& imgResult)
{
	/*
	if(!imgSource.isContinuous() || !imgResult.isContinuous())
	{
		cout<<"images are not continous"<<endl;
	}
	*/

	const int rows = imgSource.rows;
	const int cols = imgSource.cols;

	uchar* tempTable = new uchar[cols];

	int i, j;

	uchar* pSrc = imgSource.data + cols;
	uchar* pTemp;
	uchar* pDst = imgResult.data + cols + 1;
	for(i=1; i<rows-1; ++i)
	{
		uchar* pTemp = tempTable;

		// for all pixels in row calculate value of 1/9 of sum of above and below pixels
		for(j=0; j<cols; ++j)
		{
			*pTemp = (uchar) ( (unsigned int) ( *(pSrc-cols) + *pSrc + *(pSrc+cols) ) / 9 );

			pTemp++;
			pSrc++;
		}

		pTemp = tempTable;
		// for every pixel in row with exception of the two on the left and right side calc sum of temp table
		for(j=1; j<cols-1; ++j)
		{
			*pDst = (uchar) ( *pTemp + *(pTemp+1) + *(pTemp+2) );

			pTemp++;
			pDst++;
		}
		pDst += 2;
	}

	delete [] tempTable;
}

void BlurSuperFast(Mat& imgSource, Mat& imgResult)
{
	const int rows = imgSource.rows;
	const int cols = imgSource.cols;

	uchar* pSrc = imgSource.data;

	int i, j;

	if(!imgSource.isContinuous())
	{
		cout<<"source is not continous"<<endl;
	}

	if(!imgResult.isContinuous())
	{
		cout<<"result is not continous"<<endl;
	}

	uchar* pTemp = pSrc;
	uchar* pDst = imgResult.data + 1;
	// vertical pass
	for(i=0; i<rows; ++i)
	{
		for(j=1; j<cols-1; ++j)
		{
			*pDst = (uchar)(((unsigned int)(*pTemp + *(pTemp+1) + *(pTemp+2)))/3);

			pDst++;
			pTemp++;
		}
		pDst += 2;
		pTemp += 2;
	}

	//cout<<imgResult<<endl;

	pTemp = imgResult.data + cols + 1;
	// horizontal pass
	for(i=1; i<rows-1; ++i)
	{
		for(j=1; j<cols-1; ++j)
		{
			*pTemp = (uchar)((unsigned int)(*(pTemp-cols) + *pTemp + *(pTemp+cols))/3);

			pTemp++;
		}
		pTemp += 2;
	}

	//cout<<imgResult<<endl;
}


#define GET_DATA(pointer, x, y, width )  ((*((pointer)+(y)*(width)+(x))))

void BlurFast( Mat& src,  Mat& dst)
{
    const int width = src.cols;
    const int height = src.rows;

    dst = src.clone();

     uchar* pSrc = src.data;
     uchar* pDst = dst.data;

     int* tabl = new int[width];

     for(int y = 1; y < height - 1 ; ++y)
     {
         for(int x = 0; x < width; ++x)
         {
           tabl[x] = ( (int) GET_DATA(pSrc, x , y - 1 , width) + GET_DATA(pSrc, x, y, width) + GET_DATA(pSrc, x, y + 1, width))/9;
         }

         for(int x = 1; x < width - 1; ++x)
         {
             GET_DATA(pDst, x, y, width) = tabl[x-1] + tabl[x] + tabl[x+1];
         }
     }

     delete[] tabl;
}

void blurWiktor(Mat& imgSource, Mat& imgResult)
{
	int rows = imgSource.rows;
	int cols = imgSource.cols;

	const int filterSize = 3;

	int* tabl = new int[cols];

	// horizontal pass
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
    for(int i=1; i<rows-1; ++i)
    {
        const uchar* rowPrevious = imgSource.ptr<uchar>(i - 1);
        const uchar* rowCurrent = imgSource.ptr<uchar>(i);
        const uchar* rowNext = imgSource.ptr<uchar>(i + 1);

		uchar* output = imgResult.ptr<uchar>(i);

		for(int j=0; j<cols; ++j)
		{
			tabl[j] = (int)(rowPrevious[j] + rowCurrent[j] + rowNext[j])/9;
		}

        for(int j=1; j<cols-1; ++j)
        {
			int sum = (int)(tabl[j-1] + tabl[j] + tabl[j+1]);

			output[j] = saturate_cast<uchar> (sum / filterSize);
        }
    }

	delete[] tabl;
	
	uchar* output = imgResult.ptr<uchar>(0);
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int j=0; j<cols; ++j)
	{
		output[j] = 0;
	}
	output = imgResult.ptr<uchar>(rows-1);
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int j=0; j<cols; ++j)
	{
		output[j] = 0;
	}
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int i=0; i<rows; i++)
	{
		output = imgResult.ptr<uchar>(i);
		output[0] = 0;
		output[cols-1] = 0;
	}
}

void blurTwoPass(Mat& imgSource, Mat& imgResult)
{
	int rows = imgSource.rows;
	int cols = imgSource.cols;

	const int filterSize = 3;

	// horizontal pass
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
    for(int i=1; i<rows-1; ++i)
    {
        const uchar* rowCurrent = imgSource.ptr<uchar>(i);

        uchar* output = imgResult.ptr<uchar>(i);

        for(int j=1; j<cols-1; ++j)
        {
			int sum = (int)(rowCurrent[j-1] + rowCurrent[j] + rowCurrent[j+1]);

			output[j] = saturate_cast<uchar> (sum / filterSize);
        }
    }

	// vertical pass
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
    for(int i=1; i<rows-1; ++i)
    {
        const uchar* rowPrevious = imgResult.ptr<uchar>(i - 1);
        const uchar* rowCurrent = imgResult.ptr<uchar>(i);
        const uchar* rowNext = imgResult.ptr<uchar>(i + 1);

		uchar* output = imgResult.ptr<uchar>(i);

        for(int j=1; j<cols-1; ++j)
        {
			int sum = (int)(rowPrevious[j] + rowCurrent[j] + rowNext[j]);

			output[j] = saturate_cast<uchar> (sum / filterSize);
        }
    }

	uchar* output = imgResult.ptr<uchar>(0);
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int j=0; j<cols; ++j)
	{
		output[j] = 0;
	}
	output = imgResult.ptr<uchar>(rows-1);
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int j=0; j<cols; ++j)
	{
		output[j] = 0;
	}
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int i=0; i<rows; i++)
	{
		output = imgResult.ptr<uchar>(i);
		output[0] = 0;
		output[cols-1] = 0;
	}
}

void blur(Mat& imgSource, Mat& imgResult)
{
	int rows = imgSource.rows;
	int cols = imgSource.cols;

	const int filterSize = 3*3;

	//for(int i=rows-2; i!=0; --i) nie dzia³a dla openmp

	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
    for(int i=1; i<rows-1; ++i)
    {
        const uchar* rowPrevious = imgSource.ptr<uchar>(i - 1);
        const uchar* rowCurrent = imgSource.ptr<uchar>(i);
        const uchar* rowNext = imgSource.ptr<uchar>(i + 1);

        uchar* output = imgResult.ptr<uchar>(i);

        for(int j=1; j<cols-1; ++j)
        {
			int sum = (int)(rowPrevious[j-1] + rowPrevious[j] + rowPrevious[j+1] +
							rowCurrent[j-1] + rowCurrent[j] + rowCurrent[j+1] +
							rowNext[j-1] + rowNext[j] + rowNext[j+1] );

			output[j] = saturate_cast<uchar> (sum / filterSize);
        }
    }

	uchar* output = imgResult.ptr<uchar>(0);
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int j=0; j<cols; ++j)
	{
		output[j] = 0;
	}
	output = imgResult.ptr<uchar>(rows-1);
	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int j=0; j<cols; ++j)
	{
		output[j] = 0;
	}

	#ifdef USE_OPENMP
		#pragma omp parallel for
	#endif
	for(int i=0; i<rows; i++)
	{
		output = imgResult.ptr<uchar>(i);
		output[0] = 0;
		output[cols-1] = 0;
	}

	//imgResult.row(0).setTo(Scalar(0));
    //imgResult.row(rows-1).setTo(Scalar(0));
    //imgResult.col(0).setTo(Scalar(0));
    //imgResult.col(cols-1).setTo(Scalar(0));
}