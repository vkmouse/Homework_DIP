#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat adding_noise(const Mat src_img, double prob);
Mat execute_mean_filter(const Mat src_img, Size size);
Mat execute_median_filter(const Mat src_img, Size size);
Mat execute_alpha_trimmed_mean_filter(const Mat src_img, Size size, int d);

int main(int argc, char **argv)
{
	double noise_prob = 0.1;
	Mat ori_img, noise_img, mean_img, median_img, alpha_img;

	ori_img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	noise_img = adding_noise(ori_img, noise_prob);
	mean_img = execute_mean_filter(noise_img, Size(3, 3));
	median_img = execute_median_filter(noise_img, Size(3, 3));
	alpha_img = execute_alpha_trimmed_mean_filter(noise_img, Size(3, 3), 6);

	imshow("Original Image", ori_img);
	imshow("Noise Image", noise_img);
	imshow("After Mean Filter Image", mean_img);
	imshow("After Median Filter Image", median_img);
	imshow("After Alpha-trimmed Mean Filter Image", alpha_img);
	waitKey(0);

	return 0;
}
Mat adding_noise(const Mat src_img, double prob)
{
	std::default_random_engine G(std::chrono::high_resolution_clock::now().time_since_epoch().count()); // generater
	std::uniform_real_distribution<double> U(0.0, 1.0);													// uniform
	std::normal_distribution<double> N(0.0, 10.0);														// normal
	Mat dst_img = src_img.clone();
	int rows = dst_img.rows;
	int cols = dst_img.cols;
	uchar *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			double r = U(G); // r is a random number between 0 to 1.
			if (r < prob)	// impluse noise
				dst_ptr[x] = (uchar)floor(2 * (prob - r) / prob) * 255;
			else if (prob <= r && r < prob * 2) // gaussian noise
				dst_ptr[x] = (uchar)max(0, min((int)(dst_ptr[x] + N(G)), 255));
		}
	}

	return dst_img;
}
Mat execute_mean_filter(const Mat src_img, Size size)
{
	Mat dst_img = src_img.clone();
	Point anchor = Point(size.width / 2, size.height / 2); // floor(x/2)
	int rows = dst_img.rows;
	int cols = dst_img.cols;
	const uchar *src_ptr = src_img.ptr<uchar>(0);
	uchar *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			double pixel = 0.0;
			int count = 0;
			for (int v = 0; v < size.height; v++)
			{
				int _y = y + v - anchor.y; // y + y' - anchor.y
				if (_y < 0 || rows <= _y)
					continue;
				for (int u = 0; u < size.width; u++)
				{
					int _x = x + u - anchor.x; // x + x' - anchor.x
					if (_x < 0 || cols <= _x)
						continue;
					pixel += src_ptr[_x + _y * cols];
					count++;
				}
			}
			dst_ptr[x] = (uchar)(max(0, min((int)(pixel / count), 255)));
		}
	}
	return dst_img;
}
Mat execute_median_filter(const Mat src_img, Size size)
{
	Mat dst_img = src_img.clone();
	Point anchor = Point(size.width / 2, size.height / 2); // floor(x/2)
	int rows = dst_img.rows;
	int cols = dst_img.cols;
	const uchar *src_ptr = src_img.ptr<uchar>(0);
	uchar *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			std::vector<int> vec;
			for (int v = 0; v < size.height; v++)
			{
				int _y = y + v - anchor.y; // y + y' - anchor.y
				if (_y < 0 || rows <= _y)
					continue;
				for (int u = 0; u < size.width; u++)
				{
					int _x = x + u - anchor.x; // x + x' - anchor.x
					if (_x < 0 || cols <= _x)
						continue;
					vec.push_back(src_ptr[_x + _y * cols]);
				}
			}
			std::sort(vec.begin(), vec.end());
			if (vec.size() % 2 == 1)
				dst_ptr[x] = (uchar)vec[vec.size() / 2];
			else
				dst_ptr[x] = (uchar)(vec[vec.size() / 2] + vec[vec.size() / 2 - 1]) / 2;
		}
	}
	return dst_img;
}
Mat execute_alpha_trimmed_mean_filter(const Mat src_img, Size size, int d)
{
	Mat dst_img = src_img.clone();
	Point anchor = Point(size.width / 2, size.height / 2); // floor(x/2)
	int rows = dst_img.rows;
	int cols = dst_img.cols;
	const uchar *src_ptr = src_img.ptr<uchar>(0);
	uchar *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			std::vector<int> vec;
			for (int v = 0; v < size.height; v++)
			{
				int _y = y + v - anchor.y; // y + y' - anchor.y
				if (_y < 0 || rows <= _y)
					continue;
				for (int u = 0; u < size.width; u++)
				{
					int _x = x + u - anchor.x; // x + x' - anchor.x
					if (_x < 0 || cols <= _x)
						continue;
					vec.push_back(src_ptr[_x + _y * cols]);
				}
			}
			std::sort(vec.begin(), vec.end());
			for (int m = d; m >= 0; m = m - 2)
				if (vec.size() > m)
				{
					vec.erase(vec.end() - m / 2, vec.end());
					vec.erase(vec.begin(), vec.begin() + m / 2);
					int pixel = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
					dst_ptr[x] = (uchar)pixel;
					break;
				}
		}
	}
	return dst_img;
}