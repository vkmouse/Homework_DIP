#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat image_wrap_by_eight_params(Mat ori_img, Point src[], Point dst[]);
uchar bilinear_get_pixel(uchar *img_ptr, int rows, int cols, double x, double y);

int main(int argc, char **argv)
{
	Mat ori_img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	// four points for original picture (12,13), (33,683), (458,127), (448,615)
	// four points for new picture (0,0) (0,556) (446,0) (446,556)
	Point src[4] = {Point(12, 13), Point(33, 683), Point(458, 127), Point(448, 615)};
	Point dst[4] = {Point(0, 0), Point(0, 556), Point(446, 0), Point(446, 556)};

	Mat img = image_wrap_by_eight_params(ori_img, src, dst);

	imshow("Original Image", ori_img);
	imshow("Bilinear Image", img);
	waitKey(0);

	return 0;
}
Mat image_wrap_by_eight_params(Mat ori_img, Point src[], Point dst[])
{
	double x[4], y[4], u[4], v[4];
	for (int i = 0; i < 4; i++)
	{
		x[i] = src[i].x;
		y[i] = src[i].y;
		u[i] = dst[i].x;
		v[i] = dst[i].y;
	}
	double new_cols = std::max(u[2], u[3]) - std::min(u[0], u[1]);
	double new_rows = std::max(v[2], v[3]) - std::min(v[0], v[1]);
	Mat img = Mat(new_rows, new_cols, CV_8UC1);

	// AX = B -> X = A-1*B
	// A = {u[0]v[0], u[0], v[0], 1,
	//		u[1]v[1], u[1], v[1], 1,
	//		u[2]v[2], u[2], v[2], 1,
	//		u[3]v[3], u[3], v[3], 1}
	Mat A = (Mat_<double>(4, 4) << u[0] * v[0], u[0], v[0], 1,
			 u[1] * v[1], u[1], v[1], 1,
			 u[2] * v[2], u[2], v[2], 1,
			 u[3] * v[3], u[3], v[3], 1);

	// X1 = {a1, B1 = {x[0],
	//		 a2, 	   x[1],
	// 		 a3, 	   x[2],
	// 		 a4} 	   x[3]}
	Mat B1 = (Mat_<double>(4, 1) << x[0], x[1], x[2], x[3]);
	Mat X1 = A.inv() * B1;

	// X2 = {a5, B2 = {y[0],
	// 		 a6, 	   y[1],
	// 		 a7, 	   y[2],
	// 		 a8} 	   y[3]}
	Mat B2 = (Mat_<double>(4, 1) << y[0], y[1], y[2], y[3]);
	Mat X2 = A.inv() * B2;

	double *a = new double[8];
	memcpy(a, X1.ptr<double>(0), 4 * sizeof(double));
	memcpy(a + 4, X2.ptr<double>(0), 4 * sizeof(double));

	// x = a1*uv + a2*u + a3*v + a4
	// y = a5*uv + a6*u + a7*v + a8
	uchar *ori_img_ptr = ori_img.ptr<uchar>(0);
	int ori_rows = ori_img.rows;
	int ori_cols = ori_img.cols;
	for (int v = 0; v < new_rows; v++)
	{
		uchar *img_ptr = img.ptr<uchar>(v);
		for (int u = 0; u < new_cols; u++)
		{
			double x = a[0] * u * v +
					   a[1] * u +
					   a[2] * v +
					   a[3];
			double y = a[4] * u * v +
					   a[5] * u +
					   a[6] * v +
					   a[7];

			img_ptr[u] = bilinear_get_pixel(ori_img_ptr, ori_rows, ori_cols, x, y);
		}
	}
	return img;
}
uchar bilinear_get_pixel(uchar *img_ptr, int rows, int cols, double x, double y)
{
	// y and fy are position.
	// h is the distance of the diffentce.
	int fy = floor(y);
	double h = y - fy;

	// x and fx are position.
	// w is the distance of the diffentce.
	int fx = floor(x);
	double w = x - fx;

	// move point to fy
	img_ptr = img_ptr + fy * cols;

	// (fx, fy) is the top-left point.
	uchar A = img_ptr[fx];
	uchar B = img_ptr[min(fx + 1, cols - 1)];
	uchar C = img_ptr[fx + (min(fy + 1, rows - 1) - fy) * cols];
	uchar D = img_ptr[min(fx + 1, cols - 1) + (min(fy + 1, rows - 1) - fy) * cols];

	return A * (1 - w) * (1 - h) + B * w * (1 - h) + C * h * (1 - w) + D * w * h;
}