#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

void onMouse(int Event, int x, int y, int flags, void* param);
Mat region_filling(Mat image, int fill_x, int fill_y);
Mat binary_image(Mat image, int threshold);

Mat image;
int count;

int main(int argc, char** argv) {
	count = 0;
	image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	image = binary_image(image, 100);
	
	namedWindow("origin", WINDOW_AUTOSIZE);
	setMouseCallback("origin", onMouse, NULL);
	imshow("origin", image);
	waitKey(0);
	return 0;
}
Mat region_filling(Mat image, int fill_x, int fill_y)
{
	image = image.clone();
	std::vector<Point> new_active_point;
	Mat component = (Mat_<bool>(3, 3) << 0, 1, 0,
										 1, 1, 1,
										 0, 1, 0);
	new_active_point.push_back(Point(fill_x, fill_y));

	while (new_active_point.size() != 0)
	{
		int x = new_active_point.front().x;
		int y = new_active_point.front().y;
		image.at<uchar>(y, x) = 255;
		for (int m = 0; m < component.cols; m++) // x 軸
			for (int n = 0; n < component.rows; n++) // y 軸
			{
				int _x = x - 1 + m;
				int _y = y - 1 + n;
				if ((0 <= _x && _x < image.cols) && (0 <= _y && _y < image.rows))
					if (component.at<bool>(n, m) && image.at<uchar>(_y, _x) / 255 == 0)
					{
						image.at<uchar>(_y, _x) = 255;
						new_active_point.push_back(Point(_x, _y));
					}
			}
		new_active_point.erase(new_active_point.begin());
	}

	return image;
}
Mat binary_image(Mat image, int threshold)
{
	image = image.clone();
	for (int x = 0; x < image.cols; x++)
		for (int y = 0; y < image.rows; y++)
		{
			if (image.at<uchar>(y, x) > 100)
				image.at<uchar>(y, x) = 255;
			else
				image.at<uchar>(y, x) = 0;
		}
	return image;
}
void onMouse(int Event, int x, int y, int flags, void* param) {
	if (Event == CV_EVENT_LBUTTONDOWN) {
		image = region_filling(image, x, y);
		imshow("origin", image);
		// output
		char fn[256];
		count++;
		snprintf(fn, sizeof(fn), "result_%d.bmp", count);
		imwrite(fn, image);
	}
}
