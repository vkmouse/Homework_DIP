#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

void onMouse(int Event, int x, int y, int flags, void *param);
Mat hole_filling(Mat src_img, Point pos);
Mat binary_image(Mat src_img, int threshold);

Mat image;
int count;

int main(int argc, char **argv)
{
	count = 0;
	image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	image = binary_image(image, 100);

	namedWindow("origin", WINDOW_AUTOSIZE);
	setMouseCallback("origin", onMouse, NULL);
	imshow("origin", image);
	waitKey(0);
	return 0;
}

Mat hole_filling(Mat src_img, Point pos)
{
	Mat dst_img = src_img.clone();
	int rows = src_img.rows;
	int cols = src_img.cols;
	std::vector<Point> new_active_point(1, pos);
	Mat component = (Mat_<bool>(3, 3) << 0, 1, 0,
					 1, 1, 1,
					 0, 1, 0);
	bool *c_ptr = component.ptr<bool>(0);
	uchar *dst_ptr = dst_img.ptr<uchar>(0);
	dst_img.at<uchar>(pos.y, pos.x) = 255;

	while (new_active_point.size() != 0)
	{
		int x = new_active_point.front().x;
		int y = new_active_point.front().y;

		for (int v = 0; v < 3; v++)
		{
			int _y = y + v - 1; // anchor.y is 1
			if (_y < 0 || rows <= _y)
				continue;
			for (int u = 0; u < 3; u++)
			{
				int _x = x + u - 1; // anchor.x is 1
				if (_x < 0 || cols <= _x)
					continue;

				if (c_ptr[v * 3 + u] && dst_ptr[_y * cols + _x] < 255)
				{
					dst_ptr[_y * cols + _x] = 255;
					new_active_point.push_back(Point(_x, _y));
				}
			}
		}
		new_active_point.erase(new_active_point.begin());
	}

	return dst_img;
}
Mat binary_image(Mat src_img, int threshold)
{
	int cols = src_img.cols;
	int rows = src_img.rows;
	Mat dst_img = Mat(rows, cols, CV_8UC1);
	uchar *src_ptr, *dst_ptr;

	for (int y = 0; y < rows; y++)
	{
		src_ptr = src_img.ptr<uchar>(y);
		dst_ptr = dst_img.ptr<uchar>(y);
		for (int x = 0; x < cols; x++)
		{
			if (src_ptr[x] > threshold)
				dst_ptr[x] = 255;
			else
				dst_ptr[x] = 0;
		}
	}
	return dst_img;
}
void onMouse(int Event, int x, int y, int flags, void *param)
{
	if (Event == CV_EVENT_LBUTTONDOWN)
	{
		image = hole_filling(image, Point(x, y));
		imshow("origin", image);
		// output
		char fn[256];
		count++;
		snprintf(fn, sizeof(fn), "result_%d.bmp", count);
		imwrite(fn, image);
	}
}
