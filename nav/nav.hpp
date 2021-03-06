#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include "../detection/detect_objects.hpp"
#include "../tracking/conf_arc.hpp" 

struct Heading {
	uint16_t magnitude;
	uint16_t angle;
};

void sendToCtrl(Heading* heading);
void focusClosestObject(cv::Point2f* closest, cv::Point2f* origin, std::vector<cv::Point2f>* centers);
void updateHeading(Heading* heading, Prediction* pred, cv::Point2f* origin);
void averageHeadings(Heading* heading, std::vector<Heading>* heading_record);
void reportAverageHeading(Heading* heading, std::vector<Heading>* heading_record);
