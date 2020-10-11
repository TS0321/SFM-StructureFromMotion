#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "util.hpp"

class Tracker
{
public:
	Tracker() {};
	~Tracker() {};

	void setMap(Map& map) {
		this->map = &map;
	}

	void detectFeatureAndcomputeDescriptor(KeyFrame &keyframe);
	void solveMatching(KeyFrame& keyframe1, KeyFrame& keyframe2);
	void calcPosefrom2D2D(KeyFrame& keyframe1, KeyFrame& keyframe2);
	void calcPosefrom2D3D(KeyFrame& keyframe1, KeyFrame& keyframe2);
	std::vector<cv::DMatch>& getMatching() {
		return matching;
	}
private:
	std::vector<cv::DMatch> matching;
	Map* map;
};