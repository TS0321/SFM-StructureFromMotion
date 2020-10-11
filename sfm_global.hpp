#pragma once

#include <iostream>

#define SFM_GLOBAL SfmGlobal::getInstance()

typedef struct CamParam {
	int width;
	int height;
	float fx;
	float fy;
	float cx;
	float cy;
	float k1;
	float k2;
	float k3;
	float p1;
	float p2;
}CamParam;

class SfmGlobal {
public:
	static SfmGlobal& getInstance() {
		return SfmGlobalInstance;
	}

	CamParam& getCamParam() {
		return camParam;
	}
private:
	static SfmGlobal SfmGlobalInstance;
	CamParam camParam;
};

//SfmGlobal sfm_global;

//namespace SfmGlobal {
//	CamParam camParam;
//	CamParam loadCamParam(const int width, const int height, const float fx, const float fy, const float cx, const float cy, const float k1, const float k2, const float k3, const float p1, const float p2);
//}