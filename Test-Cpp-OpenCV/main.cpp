#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace std;

static const char *filename;
static const char *direname;

#define pathlen 256

bool equals(cv::Mat &m1, cv::Mat &m2, int s) {
	auto i1 = m1.begin<cv::Vec3b>();
	auto i2 = m2.begin<cv::Vec3b>();
	auto e1 = m1.end<cv::Vec3b>();
	auto e2 = m2.end<cv::Vec3b>();

	int n = 0;
	int f = 0;

	while (true) {
		if (i1 >= e1) {
			break;
		}
		else if (i2 >= e2) {
			break;
		}

		if (*i1 != *i2) {
			f++;
		}

		i1++;
		i2++;

		n++;
	}

	int r = f * 100 / n;

	if (r < s) {
		return true;
	}
	else {
		return false;
	}
}

void save(cv::Mat frame, int index) {
	char outpath[pathlen];
	sprintf_s(outpath, pathlen, "%s\\%05d.jpg", direname, index);

	cout << outpath << endl;

	cv::imwrite(outpath, frame);
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++) {
		cout << argv[i] << endl;
	}

	if (argc != 5) {
		cout << "args : Video File Name , Output Directory Name , First rate , Second rate" << endl;
		system("pause");
		return -1;
	}

	filename = argv[1];
	direname = argv[2];
	const int first = atoi(argv[3]);
	const int second = atoi(argv[4]);

	cv::VideoCapture cap(filename);

	if (!cap.isOpened()) {
		cout << "File Open Faild" << endl;
		system("pause");
		return -1;
	}

	vector<cv::Mat> buf;

	cv::Mat lastframe;
	bool same = true;

	cap >> lastframe;

	for (int i = 0; i < 100;) {
		cv::Mat frame;

		cap >> frame;

		if (frame.empty()) {
			i++;
		}
		else {
			if (equals(frame, lastframe, first)) {
				if (same) {
					buf.push_back(frame);

					same = false;
				}
			}
			else {
				same = true;
			}

			frame.copyTo(lastframe);

			i = 0;
		}
	}

	lastframe = buf[0];

	int index = 0;

	for (int i = 1; i < buf.size(); i++) {
		cv::Mat frame = buf[i];

		if (!equals(frame, lastframe, second)) {
			save(lastframe, index);

			index++;
		}

		frame.copyTo(lastframe);
	}

	save(lastframe, index);

	return 0;
}