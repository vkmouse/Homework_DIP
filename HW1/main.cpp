#include <iostream>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>
using namespace cv;

Mat nearest_neighbor_interpolation(Mat original_img, double scale);
Mat bilinear_interpolation(Mat original_img, double scale);

int main(int argc, char **argv)
{
	double scale = 2.0;
	Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat nn_img = nearest_neighbor_interpolation(img, scale);
	Mat b_img = bilinear_interpolation(img, scale);

	imshow("Original Image", img);
	imshow("Nearest Neighbor Interpolation Image", nn_img);
	imshow("Bilinear Interpolation Image", b_img);
	waitKey(0);

	return 0;
}
Mat nearest_neighbor_interpolation(Mat original_img, double scale)
{
	int ori_rows = original_img.rows;
	int ori_cols = original_img.cols;
	int new_rows = round(ori_rows * scale);
	int new_cols = round(ori_cols * scale);
	Mat img = Mat(new_rows, new_cols, CV_8UC1);

	for (int j = 0; j < new_rows; j++)
	{
		// find nearest neighbor in y-axis
		double y = j / scale;
		int idx_y, fy = floor(y);
		if (y - fy < y - fy + 1)
			idx_y = fy;
		else
			idx_y = fy + 1;
		idx_y = min(idx_y, ori_rows - 1);

		uchar *img_ptr = img.ptr<uchar>(j);
		uchar *ori_img_ptr = original_img.ptr<uchar>(idx_y);

		for (int i = 0; i < new_cols; i++)
		{
			// find nearest neighbor in x-axis
			double x = i / scale;
			int idx_x, fx = floor(x);
			if (x - fx < x - fx + 1)
				idx_x = fx;
			else
				idx_x = fx + 1;
			idx_x = min(idx_x, ori_cols - 1);

			// assign pixel
			img_ptr[i] = ori_img_ptr[idx_x];
		}
	}

	return img;
}

// A-----i--B
// ----------
// ------Y---
// ----------
// C-----j--D
// A, B, C, D, i, j, Y are pixels. w, h are distances.
// Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
Mat bilinear_interpolation(Mat original_img, double scale)
{
	int ori_rows = original_img.rows;
	int ori_cols = original_img.cols;
	int new_rows = round(ori_rows * scale);
	int new_cols = round(ori_cols * scale);
	Mat img = Mat(new_rows, new_cols, CV_8UC1);

	for (int j = 0; j < new_rows; j++)
	{
		// y and fy are position.
		// h is the distance of the diffentce.
		double y = j / scale;
		int fy = floor(y);
		double h = y - fy;

		uchar *img_ptr = img.ptr<uchar>(j);
		uchar *ori_img_ptr = original_img.ptr<uchar>(fy);

		for (int i = 0; i < new_cols; i++)
		{
			// x and fx are position.
			// w is the distance of the diffentce.
			double x = i / scale;
			int fx = floor(x);
			double w = x - fx;

			// (fx, fy) is the top-left point.
			uchar A = ori_img_ptr[fx];
			uchar B = ori_img_ptr[min(fx + 1, ori_cols - 1)];
			uchar C = ori_img_ptr[fx + (min(fy + 1, ori_rows - 1) - fy) * ori_cols];
			uchar D = ori_img_ptr[min(fx + 1, ori_cols - 1) + (min(fy + 1, ori_rows - 1) - fy) * ori_cols];

			// assign pixel
			img_ptr[i] = A * (1 - w) * (1 - h) + B * w * (1 - h) + C * h * (1 - w) + D * w * h;
		}
	}
	return img;
}
