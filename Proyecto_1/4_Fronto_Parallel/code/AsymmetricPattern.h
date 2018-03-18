#ifndef ASYMMETRICPATTERN_H
#define ASYMMETRICPATTERN_H

#include "Common.h"
#include "Camera.h"

#define HORIZONTALCENTERS 4
#define VERTICALCENTERS 11

struct AsymmetricPattern
{
	int numCornersHor;
	int numCornersVer;
	int numCircles;
	int numFrames;
	int iterativeProcess;
	Size boardSize;

	// Camera calibration
	Camera chessCamera;

	vector<vector<Point2f>> distortCenters;							// Centers of the distorted frames for each iteration
	vector<vector<Point2f>> distortCentersReverse;					// Centers of the distorted frames for the inverse process (FP -> Undistorted -> Distorted)

	vector<vector<Point2f>> undistortCenters;						// Centers of the undistorted frames for each iteration
	vector<vector<Point2f>> undistortCentersReverse;				// Centers of the undistorted frames for the inverse process (FP -> Undistorted -> Distorted)

	vector<vector<Point2f>> frontoParallelCenters;					// Centers of the fronto Parallel frames for each iteration
	vector<vector<Point2f>> barycentricCenters;						// This will be compared with distortCenters to measure the error for each iteration

	Mat grayImage, blackImage;
	vector<Mat> distortedFrames, undistortedFrames, frontoParallelFrames;
	vector<Mat> homographies;

	AsymmetricPattern()
	{
		iterativeProcess = 1;
		numCornersHor = HORIZONTALCENTERS;
		numCornersVer = VERTICALCENTERS;
		numCircles = HORIZONTALCENTERS * VERTICALCENTERS;
		boardSize = Size(HORIZONTALCENTERS, VERTICALCENTERS);
	}

	void recognizeCornersFirstTime(const vector<Mat>& frames);
	void colinearity(const vector<vector<Point2f>>& distortCenters);
	void calibrationOfCamera(const vector<vector<Point2f>>& distortCenters);
	vector<vector<Point2f>> recognizeCorners(const vector<Mat>& frames);
	void findUndistortedCenters();
	void findFrontoParallelCenters();
	void findUndistortedCentersReverse();
	void findDistortedCentersReverse();
	void showUndistortedFinal();
	void printFinalDataCamera();
	double distance_to_Line(Point2f startLine, Point2f endLine, Point2f point);

};

void AsymmetricPattern::recognizeCornersFirstTime(const vector<Mat>& frames)
{
	distortedFrames = frames;
	int cont = 0;
	numFrames = distortedFrames.size();
	blackImage = Mat::zeros(distortedFrames[cont].size(), distortedFrames[cont].type());
	grayImage = Mat::zeros(distortedFrames[cont].size(), distortedFrames[cont].type());
	vector<Point2f> corners;

	while (cont < numFrames) {
		cvtColor(distortedFrames[cont], grayImage, CV_BGR2GRAY);
		bool foundCorners = findCirclesGrid(distortedFrames[cont], boardSize, corners, CALIB_CB_ASYMMETRIC_GRID);

		// If find all inner corners
		if (foundCorners) {
			//drawChessboardCorners(distortedFrames[cont], boardSize, corners, foundCorners);
			distortCenters.push_back(corners);
		}
		else cout << "NO FIND INITIAL: " << cont << endl;

		imshow("Distorted Initial ", distortedFrames[cont]);

		int key = waitKey(10);
		if (key == 27) return;

		cont++;
	}
}

double AsymmetricPattern::distance_to_Line(Point2f startLine, Point2f endLine, Point2f point)
{
	double normalLength = _hypot(endLine.x - startLine.x, endLine.y - startLine.y);
	double distance = (double)((point.x - startLine.x) * (endLine.y - startLine.y) - (point.y - startLine.y) * (endLine.x - startLine.x)) / normalLength;
	return abs(distance);
}

void AsymmetricPattern::colinearity(const vector<vector<Point2f>>& distortCenters)
{
	/* To calculate colinearity error */
	double acumulateHor = 0.0, acumulateVer = 0.0, acumulateTotal = 0.0;

	for (int i = 1; i < numFrames; ++i) {
		for (int j = 1; j < 5; ++j) acumulateHor += distance_to_Line(distortCenters[i][0], distortCenters[i][40], distortCenters[i][j * 8]);
		for (int j = 1; j < 4; ++j) acumulateHor += distance_to_Line(distortCenters[i][3], distortCenters[i][43], distortCenters[i][j * 8 + 3]);
		for (int j = 1; j < 3; ++j) acumulateVer += distance_to_Line(distortCenters[i][0], distortCenters[i][3], distortCenters[i][j]);
		for (int j = 41; j < 43; ++j) acumulateVer += distance_to_Line(distortCenters[i][40], distortCenters[i][43], distortCenters[i][j]);
	}
	acumulateHor /= 8;
	acumulateVer /= 4;

	acumulateTotal += (acumulateHor + acumulateVer) / 2;

	cout << "\nLinearity Error: " << acumulateTotal / numFrames << endl;

}

void AsymmetricPattern::calibrationOfCamera(const vector<vector<Point2f>>& distortCenters)
{
	vector<vector<Point3f>> objectPoints;							// WorldSpace (3D)
	vector<vector<Point2f>> imagePoints;							// Image plane (2D)
	vector<Point3f> grid;

	for (int i = 0; i < VERTICALCENTERS; ++i) {
		for (int j = 0; j < HORIZONTALCENTERS; ++j) {
			grid.push_back(Point3f((float)(2 * j + i % 2), (float)i, 0));
		}
	}

	for (int i = 0; i < numFrames; ++i) {
		imagePoints.push_back(distortCenters[i]);
		objectPoints.push_back(grid);
	}

	vector<Mat> rvecs = chessCamera.rotations;									// vectgor of rotations
	vector<Mat> tvecs = chessCamera.translations;

	chessCamera.RMS = calibrateCamera(objectPoints, imagePoints, blackImage.size(), chessCamera.intrinsicParameters, chessCamera.distCoeffs, rvecs, tvecs);

	cout << "RMS # " << iterativeProcess++ << ": " << chessCamera.RMS << endl;
}


vector<vector<Point2f>> AsymmetricPattern::recognizeCorners(const vector<Mat>& frames)
{
	int cont = 0;
	vector<vector<Point2f>> cornersAll;
	vector<Point2f> corners;

	while (cont < numFrames) {
		cvtColor(frames[cont], grayImage, CV_BGR2GRAY);
		bool foundCorners = findCirclesGrid(frames[cont], boardSize, corners, CALIB_CB_ASYMMETRIC_GRID);

		// If find all inner corners
		if (foundCorners) {
			cornersAll.push_back(corners);
		}
		else {
			cout << "NO FIND: " << cont << endl;
		}

		cont++;
	}
	return cornersAll;
}

void AsymmetricPattern::findUndistortedCenters()
{
	undistortedFrames.resize(numFrames);

	for (size_t i = 0; i < numFrames; ++i) {
		undistort(distortedFrames[i], undistortedFrames[i], chessCamera.intrinsicParameters, chessCamera.distCoeffs);
	}

	undistortCenters = recognizeCorners(undistortedFrames);					// Fill undistortCenters from undistorted frames

	/* See Undistarted frames
	for (size_t i = 0; i < numFrames; ++i) {
		
		for (size_t j = 0; j < numCircles; ++j) circle(undistortedFrames[i], undistortCenters[i][j], 1, Scalar(0, 0, 255), 1, 8);
		imshow("Undistorted", undistortedFrames[i]);

		int key = waitKey(10);
		if (key == 27) return;
	}
	*/
}

void AsymmetricPattern::findFrontoParallelCenters()
{
	// Obtain the frames in fronto parallel
	frontoParallelFrames.resize(numFrames);
	homographies.resize(numFrames);
	vector<Point2f> inputQuad(4), outputQuad(4);
	float offset = 80.0;
	outputQuad[0] = Point2f(0, 0) + Point2f(offset, offset);
	outputQuad[1] = Point2f(grayImage.cols - 1, 0) + Point2f(-offset, offset);
	outputQuad[2] = Point2f(grayImage.cols - 1, grayImage.rows - 1) + Point2f(-offset, -offset);
	outputQuad[3] = Point2f(0, grayImage.rows - 1) + Point2f(offset, -offset);

	for (size_t i = 0; i < numFrames; ++i) {

		inputQuad[0] = undistortCenters[i][40]; inputQuad[1] = undistortCenters[i][0];
		inputQuad[2] = undistortCenters[i][3];  inputQuad[3] = undistortCenters[i][43];

		homographies[i] = getPerspectiveTransform(inputQuad, outputQuad);
		warpPerspective(undistortedFrames[i], frontoParallelFrames[i], homographies[i], grayImage.size());
	}

	// Find the centers from fronto parallel images
	frontoParallelCenters = recognizeCorners(frontoParallelFrames);

	/* See Undistarted frames
	for (size_t i = 0; i < numFrames; ++i) {
		for (size_t j = 0; j < numCircles; ++j) circle(frontoParallelFrames[i], frontoParallelCenters[i][j], 2, Scalar(255, 0, 0), 2, 8);
		imshow("Fronto Parallel", frontoParallelFrames[i]);

		int key = waitKey(20);
		if (key == 27) return;
	}
	*/
}


void AsymmetricPattern::findUndistortedCentersReverse()
{
	undistortCentersReverse.resize(numFrames);

	for (size_t i = 0; i < numFrames; ++i) {
		perspectiveTransform(frontoParallelCenters[i], undistortCentersReverse[i], homographies[i].inv());
	}

	/* See Undistarted frames
	for (size_t i = 0; i < numFrames; ++i) {
		for (size_t j = 0; j < numCircles; ++j) circle(undistortedFrames[i], undistortCenters[i][j], 2, Scalar(255, 0, 0), 2, 8);
		for (size_t j = 0; j < numCircles; ++j) circle(undistortedFrames[i], undistortCentersReverse[i][j], 2, Scalar(0, 0, 255), 2, 8);

		imshow("Undistorted Reverse", undistortedFrames[i]);

		int key = waitKey(20);
		if (key == 27) return;
	}
	*/
}


void AsymmetricPattern::findDistortedCentersReverse()
{
	distortCentersReverse.resize(numFrames);
	double cx = chessCamera.intrinsicParameters.at<double>(0, 2), cy = chessCamera.intrinsicParameters.at<double>(1, 2);
	double fx = chessCamera.intrinsicParameters.at<double>(0, 0), fy = chessCamera.intrinsicParameters.at<double>(1, 1);
	double k1 = chessCamera.distCoeffs.at<double>(0, 0), k2 = chessCamera.distCoeffs.at<double>(1, 0);
	double p1 = chessCamera.distCoeffs.at<double>(2, 0), p2 = chessCamera.distCoeffs.at<double>(3, 0), k3 = chessCamera.distCoeffs.at<double>(4, 0);

	for (size_t i = 0; i < numFrames; ++i) {
		for (size_t j = 0; j < numCircles; ++j) {
			float x = (undistortCentersReverse[i][j].x - cx) / fx;
			float y = (undistortCentersReverse[i][j].y - cy) / fy;

			float r_2 = x * x + y * y;

			float x_distort = x * (1 + k1 * r_2 + k2 * r_2 * r_2 + k3 * r_2 * r_2 * r_2);
			float y_distort = y * (1 + k1 * r_2 + k2 * r_2 * r_2 + k3 * r_2 * r_2 * r_2);

			x_distort += (2 * p1 * x * y + p2 * (r_2 + 2 * x * x));
			y_distort += (p1 * (r_2 + 2 * y * y) + 2 * p2 * x * y);

			x_distort = x_distort * fx + cx;
			y_distort = y_distort * fy + cy;

			distortCentersReverse[i].push_back(Point2f(x_distort, y_distort));
		}
	}

	/* See Undistarted frames */
	for (size_t i = 0; i < numFrames; ++i) {
		for (size_t j = 0; j < numCircles; ++j) circle(distortedFrames[i], distortCentersReverse[i][j], 1, Scalar(0, 0, 255), 1, 8);
		for (size_t j = 0; j < numCircles; ++j) circle(distortedFrames[i], distortCenters[i][j], 1, Scalar(255, 0, 0), 1, 8);

		imshow("Distorted Reverse", distortedFrames[i]);

		int key = waitKey(200);
		if (key == 27) return;
	}
	

	// First method
	for (int i = 0; i < numFrames; ++i) {
		for (int j = 0; j < numCircles; ++j) {
			distortCenters[i][j] = (distortCenters[i][j] + distortCentersReverse[i][j]) / 2.0;
		}
	}

	// Second method
	//distortCenters.clear();
	//distortCenters = distortCentersReverse;
	//distortCentersReverse.clear();
}

void AsymmetricPattern::showUndistortedFinal()
{
	for (size_t i = 0; i < numFrames; ++i) {
		imshow("Undistorted End", undistortedFrames[i]);

		int key = waitKey(10);
		if (key == 27) return;
	}
}

void AsymmetricPattern::printFinalDataCamera()
{
	cout << "\nTotal of iterations: " << iterativeProcess - 1 << endl;
	cout << "\nData Camera: " << endl;
	cout << "Calibration Camera Values: " << chessCamera.RMS << endl;
	cout << "\ndistortion coefficients [k1 k2 p1 p2 k3]: " << endl;
	cout << chessCamera.distCoeffs << endl;
	cout << endl;
	cout << endl;
	cout << "Intrinsic Values:" << endl;
	cout << chessCamera.intrinsicParameters << endl;
}


#endif