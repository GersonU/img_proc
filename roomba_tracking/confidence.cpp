#include "confidence.hpp"
#include <math.h>
#include <random>
#include <utility>
#include <iostream>
#include <vector>

#define OFFSET 0.000001

using namespace std;
using namespace cv;

ConfidenceArc::ConfidenceArc(Point2f* p1, Point2f* p2) {
	this->recordPoint(p1);
	this->recordPoint(p2);
	Prediction prediction(*p1, 0);
	this->predictionHistory.push_back(prediction);
	this->predictionHistory.push_back(prediction);
	this->distanceErrors.push_back(0.0);
	this->distanceErrors.push_back(0.0);
	this->angleErrors.push_back(0.0);
	this->angleErrors.push_back(0.0);
}

void ConfidenceArc::recordPoint(Point2f* p) {
	this->path.push_back(*p);
}

vector<Point2f>* ConfidenceArc::getPath() {
	return &(this->path);
}

vector<Prediction>* ConfidenceArc::getPredictionHistory() {
	return &(this->predictionHistory);
}

Prediction* ConfidenceArc::getLatestPrediction() {
	return &(this->predictionHistory.at(this->predictionHistory.size() - 1));
}

void ConfidenceArc::predictPoint(int length) {
	// Calculates the standard deviation and mean from the previous distance and angle errors
	pair<double, double> distanceStats = this->fetchDevAndMean(&(this->distanceErrors), length);
	pair<double, double> angleStats = this->fetchDevAndMean(&(this->angleErrors), length);

	// Samples the normal distribution formed by the standard deviation and mean
	double distanceError = this->fetchError(&distanceStats);
	double angleError = this->fetchError(&angleStats);

	// Straight-line prediction of the next point
	Point2f previous = this->path.at(this->path.size() - 2);
	Point2f current = this->path.at(this->path.size() - 1);
	Point2f difference = current - previous;

	// Adds the latest prediction to the history
	double distance = norm(difference);
	Prediction prediction(current + difference, this->fetchConfidence(distance, distanceError, angleError));
	this->predictionHistory.push_back(prediction);
}

double ConfidenceArc::fetchConfidence(double distance, double distanceError, double angleError) {
	distance += OFFSET;
	distanceError += OFFSET;
	angleError += OFFSET;

	// Maths
	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * pow(distance + distanceError, 2);

	return 1 - arcArea / circleArea;
}

pair<double, double> ConfidenceArc::fetchDevAndMean(vector<double>* collection, int length) {
	// Hand wavy statistical calculations
	double mean = 0, variance = 0;
	int size = collection->size();
	length = length < 0 ? size : min(length, size);

	for (int i = 1; i <= length; i++) {
		mean += collection->at(size - i) / length;
	}

	for (int i = 1; i <= length; i++) {
		variance += pow(collection->at(size - i) - mean, 2);
	}

	return make_pair(sqrt(variance / (length <= 1 ? 1 : length - 1)), mean);
}

double ConfidenceArc::fetchError(pair<double, double>* stats) {
	// More hand wavy statistical calculations
	default_random_engine sample;
	normal_distribution<double> distribution(stats->second, stats->first);
	return fabs(distribution(sample));
}

void ConfidenceArc::recordError(Point2f* previous, Point2f* current) {
	// Frames all three points in terms of previous being treated as the origin
	Point2f predicted = this->getLatestPrediction()->point - *previous;
	Point2f actual = *current - *previous;

	this->distanceErrors.push_back(fabs(norm(predicted) - norm(actual)));
	this->angleErrors.push_back(fabs(atan2(predicted.y - actual.y, predicted.x - actual.x)));
}

void ConfidenceArc::cyclePoints(Point2f* previous, Point2f* current, int length) {
	this->recordPoint(previous);
	this->recordError(previous, current);
	this->predictPoint(length);	
}
