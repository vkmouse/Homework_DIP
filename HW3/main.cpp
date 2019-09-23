#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
Mat image_convolution(Mat image, Mat kernel);
Mat merge(Mat src_img1, Mat src_img2);
Mat merge_norm(Mat src_img1, Mat src_img2);

int main(int argc, char **argv)
{
	Mat ori_img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	Mat laplacian_kern = (Mat_<double>(3, 3) << -1, -1, -1,
						  -1, 8, -1,
						  -1, -1, -1);
	Mat sobel_kern_x = (Mat_<double>(3, 3) << -1, 0, 1,
						-2, 0, 2,
						-1, 0, 1);
	Mat sobel_kern_y = (Mat_<double>(3, 3) << -1, -2, -1,
						0, 0, 0,
						1, 2, 1);
	Mat avg_kern = Mat::ones(3, 3, CV_64F) / 9.0;

	Mat laplacian_img = image_convolution(ori_img, laplacian_kern);
	Mat sobel_img_x = image_convolution(ori_img, sobel_kern_x);
	Mat sobel_img_y = image_convolution(ori_img, sobel_kern_y);
	Mat sobel_img = merge(sobel_img_x, sobel_img_y);
	Mat avg_sobel_img = image_convolution(sobel_img, avg_kern);
	Mat edge_img = merge_norm(laplacian_img, avg_sobel_img);
	Mat edge_crospening_img = merge(ori_img, edge_img);

	imshow("origin", ori_img);
	imshow("Laplacian", laplacian_img);
	imshow("Sobel_img", sobel_img);
	imshow("Avg_sobel_img", avg_sobel_img);
	imshow("Edge_img", edge_img);
	imshow("Edge_crospening_img", edge_crospening_img);
	waitKey(0);

	return 0;
}
Mat image_convolution(Mat src_img, Mat kernel)
{
	int cols = src_img.cols;
	int rows = src_img.rows;
	int kern_cols = kernel.cols;
	int kern_rows = kernel.rows;
	Point anchor = Point(kern_cols / 2, kern_rows / 2); // floor(x/2)
	Mat dst_img = Mat::zeros(rows, cols, CV_8UC1);
	uchar *src_ptr = src_img.ptr<uchar>(0);
	uchar *dst_ptr;
	double *kern_ptr = kernel.ptr<double>(0);

	for (int y = 0; y < rows; y++)
	{
		dst_ptr = dst_img.ptr<uchar>(y);

		for (int x = 0; x < cols; x++)
		{
			double pixel = 0.0;
			for (int v = 0; v < kern_rows; v++)
			{
				int _y = y + v - anchor.y; // y + y' - anchor.y
				if (_y < 0 || rows <= _y)
					continue;
				for (int u = 0; u < kern_cols; u++)
				{
					int _x = x + u - anchor.x; // x + x' - anchor.x
					if (_x < 0 || cols <= _x)
						continue;
					pixel += kern_ptr[u + v * kern_cols] * src_ptr[_x + _y * cols];
				}
			}
			dst_ptr[x] = (uchar)(max(0, min((int)pixel, 255)));
		}
	}
	return dst_img;
}
Mat merge(Mat src_img1, Mat src_img2)
{
	int cols = src_img1.cols;
	int rows = src_img1.rows;
	Mat dst_img = Mat(rows, cols, CV_8UC1);
	double pixel;
	uchar *src_ptr1, *src_ptr2, *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		src_ptr1 = src_img1.ptr<uchar>(y);
		src_ptr2 = src_img2.ptr<uchar>(y);
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			pixel = src_ptr1[x] + src_ptr2[x];
			dst_ptr[x] = (uchar)(max(0, min((int)pixel, 255)));
		}
	}
	return dst_img;
}
Mat merge_norm(Mat src_img1, Mat src_img2)
{
	int cols = src_img1.cols;
	int rows = src_img1.rows;
	Mat dst_img = Mat(rows, cols, CV_8UC1);
	double pixel;
	uchar *src_ptr1, *src_ptr2, *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		src_ptr1 = src_img1.ptr<uchar>(y);
		src_ptr2 = src_img2.ptr<uchar>(y);
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			pixel = src_ptr1[x] * src_ptr2[x] / 255.0;
			dst_ptr[x] = (uchar)(max(0, min((int)pixel, 255)));
		}
	}
	return dst_img;
}