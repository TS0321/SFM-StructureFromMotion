#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <unordered_map>

struct MapPoint {
	int id;
	Eigen::Vector3d pos;
};

struct KeyFrame {
	cv::Mat image;
	std::vector<cv::KeyPoint> obsPoints;
	cv::Mat descriptor;
	std::unordered_map<int, int> uoMap;
	Eigen::Isometry3d pose;
};

struct Map {
	std::vector<MapPoint> mapPoints;
};

class Util {
public:
	Util() {};
	~Util() {};
	static void loadImages(std::vector<std::string>& filenames, std::vector<cv::Mat>& images);
	static cv::Point2f normalize(const float fx, const float fy, const float cx, const float cy, cv::Point2f uv);
	static cv::Point2f denormalize(const float fx, const float fy, const float cx, const float cy, cv::Point2f xy);
};