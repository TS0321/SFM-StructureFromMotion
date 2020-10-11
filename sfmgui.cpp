#include "sfmgui.hpp"
#include "sfm_global.hpp"

#include <iterator>

void SfmGUI::display(GLFWwindow* window) 
{
	CamParam& cparam = SFM_GLOBAL.getCamParam();
	const int w = cparam.width;
	const int h = cparam.height;
	const double fx = static_cast<double>(cparam.fx);
	const double fy = static_cast<double>(cparam.fy);
	const double cx = static_cast<double>(cparam.cx);
	const double cy = static_cast<double>(cparam.cy);

	{
		load3D(w, h, fx, fy, cx, cy, 1.0, 100);
		double mat[4 * 4] = { 0 };
		{
			for (int i = 0; i < 4; i++) {
				mat[i * 4 + i] = 1.0;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) view_id = (view_id < 6) ? view_id + 1 : view_id;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) view_id = (view_id > 0) ? view_id - 1 : view_id;

		std::vector<KeyFrame>::iterator kf_itr = keyframes->begin() + view_id;
		Eigen::Map<Eigen::Matrix4d> mvMat(&mat[0]);
		mvMat = kf_itr->pose.matrix() * mvMat;
		cv::imshow("kf", kf_itr->image);
		cv::waitKey(1);

		glLoadMatrixd(mat);
		glPointSize(3);
		glBegin(GL_POINTS);
		glColor3f(0.0, 1.0, 0.0);
		for (int i = 0; i < map->mapPoints.size(); i++) {
			glVertex3f(map->mapPoints[i].pos.x(), map->mapPoints[i].pos.y(), map->mapPoints[i].pos.z());
		}
		glEnd();

	}
}