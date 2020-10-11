#include "sfm.hpp"

void Sfm::init() {
	mapper.setTracker(tracker);
	mapper.setMap(map);

	sfmgui.setKeyframes(keyframes);
	sfmgui.setMap(map);

	tracker.setMap(map);
}

void Sfm::execute() {

	init();

	for (auto& kf : keyframes) {
		tracker.detectFeatureAndcomputeDescriptor(kf);
	}

	tracker.solveMatching(keyframes[0], keyframes[1]);
	tracker.calcPosefrom2D2D(keyframes[0], keyframes[1]);
	mapper.createMapPoints(keyframes[0], keyframes[1]);

	for (int i = 1; i < keyframes.size() - 1; i++) {
		tracker.solveMatching(keyframes[i], keyframes[i + 1]);
		tracker.calcPosefrom2D3D(keyframes[i], keyframes[i + 1]);
		mapper.createMapPoints(keyframes[i], keyframes[i + 1]);
	}

	sfmgui.execute("window", 640, 480);
}

void Sfm::register_keyframes(std::vector<cv::Mat>& images) {
	for (const auto& image : images)
	{
		KeyFrame keyframe;
		keyframe.image = image;
		keyframes.push_back(keyframe);
	}
}
