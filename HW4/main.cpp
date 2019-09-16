#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat adding_noise(Mat image, double noise_prob);
Mat execute_mean_filter(Mat image, Size size);
Mat execute_median_filter(Mat image, Size size);
Mat execute_alpha_trimmed_mean_filter(Mat image, Size size, int d);

int main(int argc, char** argv) {

	double noise_prob = 0.1;
	Mat ori_img, noise_img, mean_img, median_img, alpha_img;

	ori_img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	noise_img = adding_noise(ori_img, noise_prob);
	mean_img = execute_mean_filter(noise_img, Size(3, 3));
	median_img = execute_median_filter(noise_img, Size(3, 3));
	alpha_img = execute_alpha_trimmed_mean_filter(noise_img, Size(3, 3), 6);

	//namedWindow("Original Image", WINDOW_AUTOSIZE);
	//namedWindow("Noise Image", WINDOW_AUTOSIZE);
	//namedWindow("After Mean Filter Image", WINDOW_AUTOSIZE);
	//namedWindow("After Median Filter Image", WINDOW_AUTOSIZE);
	//namedWindow("After Alpha-trimmed Mean Filter Image", WINDOW_AUTOSIZE);

	//imshow("Original Image", ori_img);
	//imshow("Noise Image", noise_img);
	//imshow("After Mean Filter Image", mean_img);
	//imshow("After Median Filter Image", median_img);
	//imshow("After Alpha-trimmed Mean Filter Image", alpha_img);

	imwrite("Noise Image.jpg", noise_img);
	imwrite("After Mean Filter Image.jpg", mean_img);
	imwrite("After Median Filter Image.jpg", median_img);
	imwrite("After Alpha-trimmed Mean Filter Image.jpg", alpha_img);

	// imwrite("ori_img.jpg", ori_img);
	waitKey(0);
	system("pause");
	return 0;

}
Mat adding_noise(Mat image, double noise_prob)
{
	std::default_random_engine G(std::chrono::high_resolution_clock::now().time_since_epoch().count()); // generater
	std::uniform_real_distribution<double> U(0.0, 1.0); // uniform
	std::normal_distribution<double> N(0.0, 10.0); // normal
	image = image.clone();
	int count1 = 0, count2 = 0;

	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			double r = U(G);
			int pixel = image.at<uchar>(y, x);
			if (r <= noise_prob)
			{
				// impluse noise
				if (r < noise_prob * 0.5)
					pixel = 0;
				else
					pixel = 255;
			}
			if (noise_prob < r && r <= noise_prob * 2)
			{
				// gaussian noise
				pixel = pixel + N(G);
				if (pixel > 255)
					pixel = 255;
				else if (pixel < 0)
					pixel = 0;
			}
			image.at<uchar>(y, x) = pixel;
		}

	return image;
}
Mat execute_mean_filter(Mat image, Size size)
{
	image = image.clone();
	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			double pixel = 0.0;
			int count = 0;
			for (int m = 0; m < size.width; m++) // x 軸
				for (int n = 0; n < size.height; n++) // y 軸
				{
					int _x = x - 1 + m;
					int _y = y - 1 + n;
					if ((0 <= _x && _x < image.cols) && (0 <= _y && _y < image.rows))
					{
						pixel += image.at<uchar>(_y, _x);
						count++;
					}
				}
			pixel = pixel / count;
			if (pixel > 255) pixel = 255;
			else if (pixel < 0) pixel = 0;
			image.at<uchar>(y, x) = (uchar)pixel;
		}
	return image;
}
Mat execute_median_filter(Mat image, Size size)
{
	image = image.clone();
	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			std::vector<int> vec;
			for (int m = 0; m < size.width; m++) // x 軸
				for (int n = 0; n < size.height; n++) // y 軸
				{
					int _x = x - 1 + m;
					int _y = y - 1 + n;
					if ((0 <= _x && _x < image.cols) && (0 <= _y && _y < image.rows))
						vec.push_back(image.at<uchar>(_y, _x));
				}
			std::sort(vec.begin(), vec.end());
			int pixel = vec[vec.size() / 2];
			if (vec.size() % 2 == 0)
				pixel = (vec[vec.size() / 2 - 1] + vec[vec.size() / 2]) / 2;
			image.at<uchar>(y, x) = (uchar)pixel;
		}
	return image;
}
Mat execute_alpha_trimmed_mean_filter(Mat image, Size size, int d)
{
	image = image.clone();
	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			std::vector<int> vec(size.width * size.height);
			for (int m = 0; m < size.width; m++) // x 軸
				for (int n = 0; n < size.height; n++) // y 軸
				{
					int _x = x - 1 + m;
					int _y = y - 1 + n;
					if ((0 <= _x && _x < image.cols) && (0 <= _y && _y < image.rows))
						vec[m * size.height + n] = image.at<uchar>(_y, _x);
				}
			std::sort(vec.begin(), vec.end());
			vec.erase(vec.end() - d / 2, vec.end());
			vec.erase(vec.begin(), vec.begin() + d / 2);
			int pixel = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
			image.at<uchar>(y, x) = (uchar)pixel;
		}
	return image;
}
