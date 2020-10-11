#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>


#include "sfm_global.hpp"
#include "util.hpp"
#include "sfm.hpp"

//#define SFM_GLOBAL SfmGlobal::getInstance()

int main(void)
{

	//カメラパラメータの設定
	const int width = 640;
	const int height = 480;
	const float fx = 8.093086e+02;
	const float fy = 8.106449e+02;
	const float cx = 3.199442e+02;
	const float cy = 2.785334e+02;
	const float k1 = 1.818949e-02;
	const float k2 = -2.175777e-01;
	const float k3 = 6.684171e-01;
	const float p1 = 2.078000e-02;
	const float p2 = 1.822711e-03;

	CamParam& cparam = SFM_GLOBAL.getCamParam();
	cparam.width = width;
	cparam.height = height;
	cparam.fx = fx;
	cparam.fy = fy;
	cparam.cx = cx;
	cparam.cy = cy;
	cparam.k1 = k1;
	cparam.k2 = k2;
	cparam.k3 = k3;
	cparam.p1 = p1;
	cparam.p2 = p2;


	//画像のロード
	const int img_num = 7;
	std::vector<std::string> filenames;
	for (int i = 0; i < img_num; i++) {
		filenames.emplace_back("../data/shiba0" + std::to_string(i) + ".bmp");
	}
	std::vector<cv::Mat> images;
	Util::loadImages(filenames, images);

	//Sfm
	Sfm sfm;
	sfm.register_keyframes(images);
	sfm.execute();


	return 0;
}