#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

Mat nearest_neighbor_interpolation(Mat original_img, double scale);
Mat bilinear_interpolation(Mat original_img, double scale);

int main(int argc, char** argv) {
	double scale = 10.0;
	Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat nn_img = nearest_neighbor_interpolation(img, scale);
	Mat b_img = bilinear_interpolation(img, scale);
	

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", img);
	namedWindow("Nearest Neighbor Interpolation Image", WINDOW_AUTOSIZE);
	imshow("Nearest Neighbor Interpolation Image", nn_img);
	namedWindow("Bilinear Interpolation Image", WINDOW_AUTOSIZE);
	imshow("Bilinear Interpolation Image", b_img);
	waitKey(0);

	return 0;
}
Mat nearest_neighbor_interpolation(Mat original_img, double scale)
{
	Mat img = Mat_<uchar>(original_img.rows * scale, original_img.cols * scale);
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			double x = i / scale;
			double y = j / scale;

			// find nearest neighbor
			int idx_x, idx_y;
			double min_distance = INT_MAX;

			for (int m = 0; m < 2; m++)
				for (int n = 0; n < 2; n++)
				{
					double distance = pow(x - (int)(x + m), 2) + pow(y - (int)(y + n), 2);
					if (min_distance > distance)
					{
						idx_x = (int)(x + m);
						idx_y = (int)(y + n);
						min_distance = distance;
					}
				}
			if (idx_x == original_img.rows)
				idx_x = original_img.rows - 1;
			if (idx_y == original_img.cols)
				idx_y = original_img.cols - 1;

			// assign pixel
			img.at<uchar>(i, j) = original_img.at<uchar>(idx_x, idx_y);
		}

	return img;
}
Mat bilinear_interpolation(Mat original_img, double scale)
{
	Mat img = Mat_<uchar>(original_img.rows * scale, original_img.cols * scale);
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			double x = i / scale;
			double y = j / scale;

			// calculate bilinear
			double pixel = 0.0;
			double distance[4], sum_distance = 0;
			int pixels[4];

			for (int m = 0; m < 2; m++)
				for (int n = 0; n < 2; n++)
				{
					int idx_x = int(x + m) < original_img.rows ? int(x + m) : original_img.rows - 1;
					int idx_y = int(y + n) < original_img.cols ? int(y + n) : original_img.cols - 1;

					distance[m * 2 + n] = sqrt(pow(x - idx_x, 2) + pow(y - idx_y, 2));
					pixels[m * 2 + n] = (int)original_img.at<uchar>(idx_x, idx_y);
					sum_distance += distance[m * 2 + n];
				}

			for (int m = 0; m < 4; m++)
				pixel += (double)pixels[m] * distance[m] / sum_distance;

			// assign pixel
			img.at<uchar>(i, j) = (uchar)(int)pixel;
		}

	return img;
}
