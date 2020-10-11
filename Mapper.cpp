#include "Mapper.hpp"
#include <opencv2/core/eigen.hpp>

void Mapper::setTracker(Tracker& tracker) {
	this->tracker = &tracker;
}

void Mapper::setMap(Map& map) {
	this->map = &map;
}

void Mapper::createMapPoints(KeyFrame &keyframe1, KeyFrame &keyframe2) {
	std::vector<cv::KeyPoint>& keyPoint1 = keyframe1.obsPoints;
	std::vector<cv::KeyPoint>& keyPoint2 = keyframe2.obsPoints;

	std::vector<cv::Point2f> pts1;
	std::vector<cv::Point2f> pts2;

	std::unordered_map<int, int>& um1 = keyframe1.uoMap;

	std::vector<cv::DMatch>& match = tracker->getMatching();
	for (int i = 0; i < match.size(); i++) {
		if (um1.find(match[i].queryIdx) != um1.end()) continue;

		pts1.push_back(keyPoint1[match[i].queryIdx].pt);
		pts2.push_back(keyPoint2[match[i].trainIdx].pt);
	}

	CamParam& cparam = SFM_GLOBAL.getCamParam();
	const float fx = cparam.fx;
	const float fy = cparam.fy;
	const float cx = cparam.cx;
	const float cy = cparam.cy;

	//“Á’¥“_‚ð³‹K‰»‰æ‘œÀ•WŒn‚É•ÏŠ·
	for (auto& pt : pts1) {
		pt = Util::normalize(fx, fy, cx, cy, pt);
	}
	Eigen::Matrix3d rotation1 = keyframe1.pose.rotation().matrix();
	Eigen::Vector3d translation1 = keyframe1.pose.translation().matrix();
	Eigen::MatrixXd projMat1(3, 4);
	projMat1 << rotation1, translation1;
	cv::Mat cvProjMat1;
	cv::eigen2cv(projMat1, cvProjMat1);
	//std::cout << "cvProjMat1" << cvProjMat1 << std::endl;

	for (auto& pt : pts2) {
		pt = Util::normalize(fx, fy, cx, cy, pt);
	}
	Eigen::Matrix3d rotation2 = keyframe2.pose.rotation().matrix();
	Eigen::Vector3d translation2 = keyframe2.pose.translation().matrix();
	Eigen::MatrixXd projMat2(3, 4);
	projMat2 << rotation2, translation2;
	cv::Mat cvProjMat2;
	cv::eigen2cv(projMat2, cvProjMat2);
	//std::cout << "cvProjMat2" << cvProjMat2 << std::endl;

	cv::Mat points4D;
	cv::triangulatePoints(cvProjMat1, cvProjMat2, pts1, pts2, points4D);

	std::cout << points4D.size() << std::endl;
	for (int i = 0; i < points4D.cols; i++) {
		Eigen::Vector3f pos(0, 0, 0);
		pos.x() = points4D.at<float>(0, i) / points4D.at<float>(3, i);
		pos.y() = points4D.at<float>(1, i) / points4D.at<float>(3, i);
		pos.z() = points4D.at<float>(2, i) / points4D.at<float>(3, i);
	
		Eigen::Vector4d point3D(pos.x(), pos.y(), pos.z(), 1);
		point3D = keyframe1.pose.matrix().inverse() * point3D.cast<double>();
		MapPoint mp;
		mp.pos = Eigen::Vector3d(point3D.x(), point3D.y(), point3D.z());
		mp.id = map->mapPoints.size();
		//std::cout << "mp : " << mp.pos << std::endl;
		map->mapPoints.push_back(mp);
		keyframe1.uoMap.insert(std::make_pair(match[i].queryIdx, mp.id));
		keyframe2.uoMap.insert(std::make_pair(match[i].trainIdx, mp.id));
	}

	std::cout << "map_size : " << map->mapPoints.size() << std::endl;
}