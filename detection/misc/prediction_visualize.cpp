#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <map>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <time.h>
#include <math.h>
using namespace std;
using namespace cv;

queue<Point2f> generatePoints() {
	srand(time(NULL));
	int x=320;
	int y=240;
	queue<Point2f> q;
	for(int i = 0; i<1000; i++ ) {
		int xoff = (rand() % 51 - 25);
		int yoff = (rand() % 51 - 25);
		x +=xoff;
		if(x>640 || x<0){
		x -= (2*xoff);
		}
		y +=yoff;
		if(y>480 || y<0){
		y -= (2*yoff);
		}
		q.push(Point2f(x,y));	
	}
	return q;
}

int display(char *title[], queue<Point2f> points){
	char *window_name;
	window_name= title[1];

	Mat image = cvarrToMat(cvCreateMat(480,640, CV_8UC3));
	namedWindow(window_name, 1);

	double PI = 3.14159265;
	Point2f old;
	Point2f current;
	Point2f next;
	srand(time(NULL));
	Point2f lastEnd;
	double lastEllipse[] = {0.0, 0.0, 0.0};
	double angle = 0;
	for(int i=0; i<points.size(); i++){
		
		if(i>0)
		{
			lastEnd = next;
			old=current;
			lastEllipse[0] = current.x;
			lastEllipse[1] = current.y;
			lastEllipse[2] = angle;
		} else {
			old = points.front();	
			lastEnd = points.front();	
		}
		
		Point2f pt = points.front();
		points.pop();
		current = pt;
		
		double x = current.x - old.x + 0.0000001;
		double y = current.y - old.y + 0.0000001;
		next=Point2f((current.x+x),(current.y+y));

		double dist = hypot(x, y);
		angle = (((x/abs(x))-1)*90) + (((y/abs(y))*(x/abs(x)))*(acos(abs(x)/dist)*(180/PI)));
		Scalar color = Scalar((rand()%200)+55,(rand()%200)+55, (rand()%200)+55);
		
		ellipse(image, Point2f(lastEllipse[0],lastEllipse[1]), Size(25,25) , lastEllipse[2], -22.5, 22.5, Scalar(0,0,0), 2);
		arrowedLine(image, old, lastEnd, Scalar(0,0,0), 2, 8, 0, 1);
		
		ellipse(image, current, Size(25,25) , angle, -22.5, 22.5, color, 2);
		arrowedLine(image, current, next, color, 2, 8, 0, 1);
		imshow(window_name, image);
		cvWaitKey(250);
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if(argc<1){
		return 1;
	}
	queue<Point2f> q = generatePoints();
	display(argv, q);
	
	return 0;
}
