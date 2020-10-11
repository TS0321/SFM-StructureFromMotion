#pragma once

#include "window.hpp"
#include "util.hpp"

class SfmGUI : public Window
{
public:
	SfmGUI() {};
	~SfmGUI() {};

	virtual void display(GLFWwindow* window);
	void setKeyframes(std::vector<KeyFrame>& keyframes) {
		this->keyframes = &keyframes;
	}
	void setMap(Map& map) {
		this->map = &map;
	}
private:
	std::vector<KeyFrame>* keyframes;
	Map* map;
	int view_id = 0;
};