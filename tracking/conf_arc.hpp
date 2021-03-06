#ifndef CONF_ARC_HPP
#define CONF_ARC_HPP

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

struct Prediction {
	double confidence, radius, range;
	cv::Point2f point;

	Prediction();
	Prediction(cv::Point2f pt, double conf, double rad, double rng);
	void setRange(double rng);
};

class ConfidenceArc {
	public:
		ConfidenceArc();
		ConfidenceArc(cv::Point2f prev, cv::Point2f curr);
		ConfidenceArc(cv::Point2f prev, cv::Point2f curr, int num);
		std::vector<cv::Point2f>* getPath();
		Prediction* getPrediction();
		std::vector<double>* getErrors();
		cv::Point2f* getPrevious();
		cv::Point2f* getCurrent();
		int* getBacktrace();
		void setBacktrace(int num);
		void predictNextFrame(cv::Point2f* curr);

	private:
		cv::Point2f prev, curr;
		int backtrace;
		std::vector<double> errors;
		std::vector<cv::Point2f> path;
		Prediction prediction;

		void recordError(cv::Point2f p1, cv::Point2f p2);
		void predict();
		void predictPoint();
		void predictRadius();
		void predictConfidence();
};

#endif
