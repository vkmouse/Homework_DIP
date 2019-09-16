#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

uchar bilinear_get_pixel(Mat img, double x, double y);

int main(int argc, char** argv) {
	Mat ori_img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	std::cout << ori_img.rows << "\t" << ori_img.cols << std::endl;

	// x = a1*uv + a2*u + a3*v + a4
	// y = a5*uv + a6*u + a7*v + a8

	// AX = B -> X = A-1*B

	double x[4] = {12, 33, 458, 448};
	double y[4] = {13, 683,	127, 615};
	double u[4] = {0, 0, 446, 446};
	double v[4] = {0, 556, 0, 556};
	Mat img = Mat_<uchar>(556, 446);

	Mat A = (Mat_<double>(4, 4) <<  u[0]*v[0], u[0], v[0], 1,
									u[1]*v[1], u[1], v[1], 1, 
									u[2]*v[2], u[2], v[2], 1, 
									u[3]*v[3], u[3], v[3], 1);
	Mat para_1 = A.inv() * Mat(4, 1, CV_64F, x);
	Mat para_2 = A.inv() * Mat(4, 1, CV_64F, y);

	for (int u = 0; u < 446; u++)
		for (int v = 0; v < 556; v++)
		{
			double x =  para_1.at<double>(0, 0) * u * v +
						para_1.at<double>(1, 0) * u +
						para_1.at<double>(2, 0) * v +
						para_1.at<double>(3, 0);
			double y =  para_2.at<double>(0, 0) * u * v +
						para_2.at<double>(1, 0) * u +
						para_2.at<double>(2, 0) * v +
						para_2.at<double>(3, 0);
			
			img.at<uchar>(v, u) = bilinear_get_pixel(ori_img, x, y);
		}
	imwrite("output.jpg", img);
	namedWindow("Bilinear Image", WINDOW_AUTOSIZE);
	imshow("Bilinear Image", img);
	waitKey(0);

	return 0;
}
uchar bilinear_get_pixel(Mat img, double x, double y)
{
	uchar pixels[4], pixel = 0.0;
	double distance[4], sum_distance = 0;

	for (int m = 0; m < 2; m++)
		for (int n = 0; n < 2; n++)
		{
			int idx_x = int(x + m) < img.cols ? int(x + m) : img.cols - 1;
			int idx_y = int(y + n) < img.rows ? int(y + n) : img.rows - 1;

			distance[m * 2 + n] = sqrt(pow(x - idx_x, 2) + pow(y - idx_y, 2));
			pixels[m * 2 + n] = img.at<uchar>(idx_y, idx_x);
			sum_distance += distance[m * 2 + n];
		}

	for (int m = 0; m < 4; m++)
		pixel += pixels[m] * (distance[m] / sum_distance);

	return pixel;
}
