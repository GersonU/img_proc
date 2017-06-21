#include "cpu_color_video.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	char* window_name;
	CvCapture* video = NULL;
	IplImage* frame = NULL;
	int threshold_value = 20, threshold_type = 2;
	int const max_BINARY_value = 255;

	if (argc > 1) {
		video = cvCaptureFromFile(argv[1]);
	} else {
		cout << "Usage: " << argv[0] << " VIDEO_FILE" << endl;
		return 1;
	}

	if (!video) {
		cout << "ERROR: Unable to open video file" << endl;
		return 1;
	}

	window_name = argv[1];
	// namedWindow(window_name, 1);
	Point2f centers[10] = { };
	frame = cvQueryFrame(video);
	int frame_count = 1;

//	for (int i = 0; i < 50; i++) {
//		cout << "QUERIED " << i << endl;
//		frame = cvQueryFrame(video);
//	}

	cout << "FINISHED FOR LOOP" << endl;

	while (frame) {
		cout << "PRE-FETCH" << endl;
		int count = fetchCenters(centers, frame);
		cout << "POST-FETCH" << endl;

		cout << frame_count << ":\t";
		for (int i = 0 ; i < 10; i++) {
			cout << "(" << centers[i].x << ", " << centers[i].y << ")\t";
		}

		cout << endl;
		frame = cvQueryFrame(video);
		frame_count++;
	}

	return 0;
}
