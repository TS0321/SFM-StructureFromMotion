#include "util.hpp"
#include <opencv2/opencv.hpp>

void Util::loadImages(std::vector<std::string>& filenames, std::vector<cv::Mat>& images) {
	for (const auto& filename : filenames)
	{
		cv::Mat image = cv::imread(filename);
		cv::waitKey(10);
		images.push_back(image);
		std::cout << "\"" << filename << "\"‚ªƒ[ƒh‚Å‚«‚Ü‚µ‚½B" << std::endl;
	}
}

cv::Point2f Util::normalize(const float fx, const float fy, const float cx, const float cy, cv::Point2f uv)
{
	float x = (uv.x - cx) / fx;
	float y = (uv.y - cy) / fy;
	cv::Point2f out_pt(x, y);

	return out_pt;
}

cv::Point2f Util::denormalize(const float fx, const float fy, const float cx, const float cy, cv::Point2f xy)
{
	float u = xy.x * fx + cx;
	float v = xy.y * fy + cy;
	cv::Point2f out_uv(u, v);

	return out_uv;
}