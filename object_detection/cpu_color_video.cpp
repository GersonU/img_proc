#include <stdio.h>
#include <iostream>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

int findLargest(int size, vector<float> radii, float max_radii[], int max[], Point2f max_centers[], vector<Point2f> centers) {
	int flag = 0, count = 0;

	for (int i = 0; i < size; i++) {
		flag = 0;

		for (int j = 0; j < 5; j++) {
			if (radii[i] > max_radii[j] && flag == 0 && radii[i] > 20) {
				max_radii[j] = radii[i];
				max[j] = i;
				max_centers[j] = centers[i];
				flag = 1;
				count++;
			}
		}
	}

	return count;
}

int CenterTracking(Point2f Centers[], IplImage *frame) {
	// calculate size of array
	int size = sizeof (Centers) / sizeof (Point2f);
	IplImage *blobs = NULL;

	// check frame was correctly loaded
	if (blobs == NULL) {
		printf("error");
		return -1;
	}

	blobs = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	Mat tempFrame, redBlobs1, redBlobs2, greenBlobs;
	vector<vector<Point>> redContours;
	vector<vector<Point>> greenContours;
	vector<Vec4i> rHierarchy;
	vector<Vec4i> gHierarchy;

	// Creating a Mat version of the current frame
	cvarrToMat(frame).copyTo(tempFrame);
	cvarrToMat(blobs).copyTo(redBlobs1);
	cvarrToMat(blobs).copyTo(redBlobs2);
	cvarrToMat(blobs).copyTo(greenBlobs);
	blur(tempFrame, tempFrame, Size(3, 3));	

	// Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
	// Uses two threshold and a aperture parameter for Sobel operator.
	cvtColor(tempFrame, redBlobs1, CV_BGR2HSV);
	cvtColor(tempFrame, redBlobs2, CV_BGR2HSV);
	cvtColor(tempFrame, greenBlobs, CV_BGR2HSV);

	// Finding the color blobs (needs to be a colored image) using Inrange
	inRange(redBlobs1, Scalar(0, 150, 150), Scalar(25, 250, 250), redBlobs1);
	inRange(redBlobs2, Scalar(155, 70, 70), Scalar(180, 200, 200), redBlobs2);
	Mat redBlobs;
	bitwise_or(redBlobs1, redBlobs2, redBlobs);
	inRange(greenBlobs, Scalar(50, 80, 80), Scalar(130, 255, 255), greenBlobs);
	findContours(redBlobs, redContours, rHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(greenBlobs, greenContours, gHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// calculate min enclosing circles for every red and green contour
	vector<vector<Point>> contours_poly(greenContours.size());
	vector<vector<Point>> contours_poly2(redContours.size());
	vector<Point2f> center(greenContours.size());
	vector<Point2f> center2(redContours.size());
	vector<float> radius(greenContours.size());
	vector<float> radius2(redContours.size());

	for (int i = 0; i < redContours.size(); i++) {
		approxPolyDP(Mat(redContours[i]), contours_poly2[i], 3, true);
		minEnclosingCircle((Mat) contours_poly2[i], center2[i], radius2[i]);
	}

	for (int i = 0;i < greenContours.size(); i++) {
		approxPolyDP(Mat(greenContours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((Mat) contours_poly[i], center[i], radius[i]);
	}

	float GreenMaxRadius[5] = {};
	float RedMaxRadius[5] = {};
	int GreenMax[5] = {};
	int RedMax[5] = {};
	Point2f GreenCenters[5];
	Point2f RedCenters[5];

	int red_count = findLargest(redContours.size(), radius2, RedMaxRadius, RedMax, RedCenters, center2);
	int green_count = findLargest(greenContours.size(), radius, GreenMaxRadius, GreenMax, GreenCenters, center);
	int total_count = red_count + green_count;

	// combine red and green arrays into one array and fill rest of array with -1,-1
	for (int i = 0; i < size; i++) {
		if (i < red_count) {
			Centers[i] = RedCenters[i];
		} else if (i < total_count) {
			Centers[i] = GreenCenters[i - red_count];
		} else {
			Centers[i] = Point2f(-1, -1);
		}
	}

	// return number of detected objects
	return total_count;
}
