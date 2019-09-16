#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
Mat Image_convolution(Mat image, Mat filter, double weight);
Mat Merge_image(Mat img1, Mat img2);
Mat Merge_image_norm(Mat img1, Mat img2);

int main(int argc, char** argv) {
	Mat ori_img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat LP_filter = (Mat_<char>(3, 3) <<-1,	-1,	-1, 
										-1,	8,	-1, 
										-1,	-1,	-1);
	Mat LP_img = Image_convolution(ori_img, LP_filter, 1.0);
	Mat Sobel_filter_x = (Mat_<char>(3, 3) <<	-1,	0,	1,
												-2,	0,	2,
												-1,	0,	1);
	Mat Sobel_filter_y = (Mat_<char>(3, 3) <<	-1,	-2,	-1,
												0,	0,	0,
												1,	2,	1);
	Mat Sobel_img_x = Image_convolution(ori_img, Sobel_filter_x, 1.0);
	Mat Sobel_img_y = Image_convolution(ori_img, Sobel_filter_y, 1.0);
	Mat Sobel_img = Merge_image(Sobel_img_x, Sobel_img_y);
	Mat Avg_filter = (Mat_<char>(3, 3) <<	1, 1, 1,
											1, 1, 1,
											1, 1, 1);
	Mat Avg_sobel_img = Image_convolution(Sobel_img, Avg_filter, 1 / 9.0);
	Mat Edge_img = Merge_image_norm(LP_img, Avg_sobel_img);

	Mat Edge_crospening_img = Merge_image(ori_img, Edge_img);

	imwrite("Laplacian.jpg", LP_img);
	imwrite("Sobel.jpg", Sobel_img);
	imwrite("Average.jpg", Avg_sobel_img);
	imwrite("Edge.jpg", Edge_img);
	imwrite("Edge_crospening.jpg", Edge_crospening_img);
	/*
	imshow("origin", ori_img);
	imshow("Laplacian", LP_img);
	imshow("Sobel_img", Sobel_img);
	imshow("Avg_sobel_img", Avg_sobel_img);
	imshow("Edge_img", Edge_img);
	imshow("Edge_crospening_img", Edge_crospening_img);*/
	waitKey(0);

	return 0;
}
Mat Image_convolution(Mat image, Mat filter, double weight)
{
	Mat output = Mat::zeros(image.rows, image.cols, CV_8UC1);// Mat_<uchar>(ori_img.rows, ori_img.cols);
	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			double pixel = 0.0;
			for (int m = 0; m < filter.cols; m++) // x 軸
				for (int n = 0; n < filter.rows; n++) // y 軸
				{
					int _x = x - 1 + m;
					int _y = y - 1 + n;
					if ((0 <= _x && _x < image.cols) && (0 <= _y && _y < image.rows))
						pixel += filter.at<char>(n, m) * image.at<uchar>(_y, _x) * weight;
				}
			if (pixel > 255) pixel = 255;
			else if (pixel < 0) pixel = 0;
			output.at<uchar>(y, x) = (uchar)pixel;
		}
	return output;
}
Mat Merge_image(Mat img1, Mat img2)
{
	Mat image = Mat_<uchar>(img1.rows, img1.cols);

	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			short pixel = img1.at<uchar>(y, x) + img2.at<uchar>(y, x);
			if (pixel > 255) pixel = 255;
			else if (pixel < 0) pixel = 0;
			image.at<uchar>(y, x) = (uchar)pixel;
		}
	return image;
}
Mat Merge_image_norm(Mat img1, Mat img2)
{
	Mat image = Mat_<uchar>(img1.rows, img1.cols);

	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			short pixel = img1.at<uchar>(y, x) * (img2.at<uchar>(y, x) / 200.0);
			if (pixel > 255) pixel = 255;
			else if (pixel < 0) pixel = 0;
			image.at<uchar>(y, x) = (uchar)pixel;
		}
	return image;
}
