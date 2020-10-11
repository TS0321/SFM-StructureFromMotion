#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

#include "Tracker.hpp"
#include "Mapper.hpp"
#include "util.hpp"
#include "sfmgui.hpp"

class Sfm {
public:
	Sfm() {};
	~Sfm() {};

	void register_keyframes(std::vector<cv::Mat>& images);
	void init();
	void execute();
private:
	std::vector<KeyFrame> keyframes;
	Map map;
	Tracker tracker;
	Mapper mapper;
	SfmGUI sfmgui;

};
