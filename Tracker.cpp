#include "Tracker.hpp"
#include "sfm_global.hpp"
#include <opencv2/core/eigen.hpp>


void Tracker::detectFeatureAndcomputeDescriptor(KeyFrame& keyframe)
{
	cv::Mat& image = keyframe.image;

	auto algorithm = cv::AKAZE::create();

	//detectFeature
	std::vector<cv::KeyPoint>& keyPoints = keyframe.obsPoints;
	algorithm->detect(image, keyPoints);

	//特徴記述
	cv::Mat& descriptor = keyframe.descriptor;
	algorithm->compute(image, keyPoints, descriptor);
}

void Tracker::solveMatching(KeyFrame& keyframe1, KeyFrame& keyframe2)
{
	cv::Mat& descriptor1 = keyframe1.descriptor;
	cv::Mat& descriptor2 = keyframe2.descriptor;

	//マッチング
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
	std::vector<cv::DMatch> match12, match21;
	matcher->match(descriptor1, descriptor2, match12);
	matcher->match(descriptor2, descriptor1, match21);

	//クロスチェック
	std::vector<cv::DMatch>& match = this->matching;
	match.clear();
	for (int i = 0; i < match12.size(); i++) {
		cv::DMatch forward = match12[i];
		cv::DMatch backward = match21[forward.trainIdx];
		if (backward.trainIdx == forward.queryIdx)
		{
			match.push_back(forward);
		}
	}

	////結果の描画
	//cv::Mat dest;
	//cv::drawMatches(keyframe1.image, keyframe1.obsPoints, keyframe2.image, keyframe2.obsPoints, match, dest);
	//cv::imshow("dest", dest);
	//cv::waitKey(0);
}

void Tracker::calcPosefrom2D2D(KeyFrame& keyframe1, KeyFrame& keyframe2)
{
	CamParam& cparam = SFM_GLOBAL.getCamParam();
	const float focal = (cparam.fx + cparam.fy) / 2;
	const cv::Point2d principalPoint(cparam.cx, cparam.cy);

	std::vector<cv::KeyPoint>& keyPoints1 = keyframe1.obsPoints;
	std::vector<cv::KeyPoint>& keyPoints2 = keyframe2.obsPoints;
	
	std::vector<cv::Point2d> pts1;
	std::vector<cv::Point2d> pts2;

	//対応特徴点のリストを作成
	for (int i = 0; i < matching.size(); i++) {
		pts1.emplace_back(keyPoints1[matching[i].queryIdx].pt);
		pts2.emplace_back(keyPoints2[matching[i].trainIdx].pt);
	}

	cv::Mat E, R, t, mask;
	E = cv::findEssentialMat(pts1, pts2, focal, principalPoint, cv::RANSAC, 0.999, 1.0, mask);
	cv::recoverPose(E, pts1, pts2, R, t, focal, principalPoint, mask);

	//初期フレームの視点
	Eigen::Isometry3d& pose1 = keyframe1.pose;
	pose1.setIdentity();
	
	//2フレーム目の視点
	Eigen::Isometry3d& pose2 = keyframe2.pose;
	pose2.setIdentity();
	Eigen::Matrix3d rotation;
	Eigen::Vector3d tvec;
	cv::cv2eigen(t, tvec);
	cv::cv2eigen(R, rotation);
	Eigen::Matrix4d transMat = Eigen::Matrix4d::Identity();
	transMat << rotation, tvec;
	pose2.matrix() = transMat;
	//std::cout << transMat << std::endl;
}

void Tracker::calcPosefrom2D3D(KeyFrame& keyframe1, KeyFrame& keyframe2)
{
	CamParam& cparam = SFM_GLOBAL.getCamParam();
	cv::Mat cvCamMat = (cv::Mat_<double>(3, 3)
		<< cparam.fx, 0, cparam.cx,
		0, cparam.fy, cparam.cy,
		0, 0, 1);
	cv::Mat cvDistCoeffs;

	std::vector<cv::Point3d> pts3D;
	std::vector<cv::Point2d> pts2D;
	
	std::vector<cv::DMatch>& match = this->matching;
	std::unordered_map<int, int>& um1 = keyframe1.uoMap;
	std::unordered_map<int, int>& um2 = keyframe2.uoMap;
	std::vector<cv::KeyPoint>& keyPoint2 = keyframe2.obsPoints;

	for (int i = 0; i < matching.size(); i++) {
		if (um1.find(match[i].queryIdx) == um1.end()) continue;
		Eigen::Vector3d& map_pos = map->mapPoints[um1[match[i].queryIdx]].pos;
		cv::Point3d pt3D(map_pos.x(), map_pos.y(), map_pos.z());
		pts3D.push_back(pt3D);

		cv::Point2f& kp = keyPoint2[match[i].trainIdx].pt;
		cv::Point2d pt2D(static_cast<double>(kp.x), static_cast<double>(kp.y));
		pts2D.push_back(pt2D);

		int& map_id = map->mapPoints[um1[match[i].queryIdx]].id;
		um2.insert(std::make_pair(match[i].trainIdx, map_id));
	}

	Eigen::Matrix3d rotation = keyframe1.pose.rotation().matrix();
	Eigen::Vector3d translation = keyframe2.pose.translation().matrix();
	
	cv::Mat R, t;
	cv::Mat rvec;
	cv::eigen2cv(rotation, R);
	cv::eigen2cv(translation, t);
	cv::Rodrigues(R, rvec);

	cv::solvePnPRansac(pts3D, pts2D, cvCamMat, cvDistCoeffs, rvec, t, true);
	cv::Rodrigues(rvec, R);
	cv::cv2eigen(R, rotation);
	cv::cv2eigen(t, translation);
	Eigen::Matrix4d transMat = Eigen::Matrix4d::Identity();
	transMat << rotation, translation;
	keyframe2.pose.matrix() = transMat;
}