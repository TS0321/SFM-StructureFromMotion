#pragma once

#include "Tracker.hpp"
#include "util.hpp"
#include "sfm_global.hpp"
class Mapper 
{
public:
	Mapper() {};
	~Mapper() {};

	void setTracker(Tracker& tracker);
	void setMap(Map& map);
	void createMapPoints(KeyFrame &keyframe1, KeyFrame &keyframe2);
private:
	Tracker* tracker = nullptr;
	Map* map = nullptr;
};