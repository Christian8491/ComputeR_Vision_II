#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"

struct Camera
{
	Mat intrinsicParameters;
	Mat distCoeffs;
	vector<Mat> rotations;
	vector<Mat> translations;
	double RMS;

	Camera(){

		intrinsicParameters = Mat(3, 3, CV_64F);
		distCoeffs = Mat(5, 1, CV_64F);
	}
};

#endif