#ifndef RINGPATTERN_H
#define RINGPATTERN_H

#include "Common.h"
#include "Utils.h"
#include "Camera.h"

#define HORIZONTALCENTERSRINGS 5
#define VERTICALCENTERSRINGS 4

struct RingPattern
{
	struct Ring
	{
		int id;
		Point2f center;

		Ring(){}
	};

	int numCornersHor;
	int numCornersVer;
	int numCircles;
	int numFrames;
	int iterativeProcess;
	Size boardSize;
	int thresh = 60;

	// Camera calibration
	Camera chessCamera;
	vector<Ring> rings;
	vector<Point2f> twentyCenters;

	vector<vector<Point2f>> distortCenters;							// Centers of the distorted frames for each iteration
	vector<vector<Point2f>> distortCentersReverse;					// Centers of the distorted frames for the inverse process (FP -> Undistorted -> Distorted)

	vector<vector<Point2f>> undistortCenters;						// Centers of the undistorted frames for each iteration
	vector<vector<Point2f>> undistortCentersReverse;				// Centers of the undistorted frames for the inverse process (FP -> Undistorted -> Distorted)

	vector<vector<Point2f>> frontoParallelCenters;					// Centers of the fronto Parallel frames for each iteration
	vector<vector<Point2f>> barycentricCenters;						// This will be compared with distortCenters to measure the error for each iteration

	Mat grayImage, blackImage, binaryAdaptative;
	vector<Mat> distortedFrames, undistortedFrames, frontoParallelFrames;
	vector<Mat> homographies;

	RingPattern()
	{
		iterativeProcess = 1;
		numCornersHor = HORIZONTALCENTERSRINGS;
		numCornersVer = VERTICALCENTERSRINGS;
		numCircles = HORIZONTALCENTERSRINGS * VERTICALCENTERSRINGS;
		boardSize = Size(HORIZONTALCENTERSRINGS, VERTICALCENTERSRINGS);
		rings.resize(HORIZONTALCENTERSRINGS * VERTICALCENTERSRINGS);
	}

	double distance_to_Line(Point2f startLine, Point2f endLine, Point2f point);
	double euclidian(const Point2f& pos1, const Point2f& pos2);
	void fillRings(vector<Point2f>& centers);
	bool recognition();
	

	void recognizeCornersFirstTime(const vector<Mat>& frames);
	void colinearity(const vector<vector<Point2f>>& distortCenters);
	void calibrationOfCamera(const vector<vector<Point2f>> distortCenters);
	vector<vector<Point2f>> recognizeCorners(const vector<Mat>& frames, bool frontoPar);
	void findUndistortedCenters();
	void findFrontoParallelCenters();
	void findUndistortedCentersReverse();
	void findDistortedCentersReverse();
	void showUndistortedFinal();
	void printFinalDataCamera();
	
};


double RingPattern::distance_to_Line(Point2f startLine, Point2f endLine, Point2f point)
{
	double normalLength = _hypot(endLine.x - startLine.x, endLine.y - startLine.y);
	double distance = (double)((point.x - startLine.x) * (endLine.y - startLine.y) - (point.y - startLine.y) * (endLine.x - startLine.x)) / normalLength;
	return abs(distance);
}


double RingPattern::euclidian(const Point2f& pos1, const Point2f& pos2)
{
	return sqrtf(powf(pos1.x - pos2.x, 2) + powf(pos1.y - pos2.y, 2));
}


void RingPattern::fillRings(vector<Point2f>& centers)
{
	for (size_t i = 0; i < rings.size(); ++i) {
		rings[i].id = i;
		rings[i].center = centers[i];
	}

	if (rings[0].center.x < rings[1].center.x) {
		for (int i = 0; i < 5; ++i)   rings[i].center = centers[4 - i];
		for (int i = 5; i < 10; ++i)  rings[i].center = centers[14 - i];
		for (int i = 10; i < 15; ++i) rings[i].center = centers[24 - i];
		for (int i = 15; i < 20; ++i) rings[i].center = centers[34 - i];
	}
}

bool RingPattern::recognition()
{
	Mat cannyOutput;
	vector<vector<Point>> contours;
	vector<RotatedRect> rectanglesRot;

	Canny(binaryAdaptative, cannyOutput, thresh, thresh * 2, 3);									// Detect edges using Threshold
	findContours(cannyOutput, contours, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));			// Find contours

	for (size_t i = 0; i < contours.size(); ++i) {
		if (contours[i].size() > 5) rectanglesRot.push_back(fitEllipse(contours[i]));
	}
	twentyCenters.clear();

	for (size_t i = 0; i < rectanglesRot.size(); ++i) {
		int contador = 0;
		Point2f auxCenter(0.0, 0.0);
		for (size_t j = i + 1; j < rectanglesRot.size(); ++j) {
			if (euclidian(rectanglesRot[i].center, rectanglesRot[j].center) < 1.0) {
				auxCenter += rectanglesRot[j].center;
				contador++;
			}
		}
		if (contador == 3) twentyCenters.push_back(auxCenter * 0.33333);
	}


	if (twentyCenters.size() == 20) {
		fillRings(twentyCenters);
		return true;
	}
	return false;
}

void RingPattern::recognizeCornersFirstTime(const vector<Mat>& frames)
{

	distortedFrames = frames;
	int cont = 0;
	numFrames = distortedFrames.size();
	vector<Point2f> centers(HORIZONTALCENTERSRINGS * VERTICALCENTERSRINGS);

	blackImage = Mat::zeros(distortedFrames[cont].size(), distortedFrames[cont].type());
	grayImage = Mat::zeros(distortedFrames[cont].size(), distortedFrames[cont].type());
	binaryAdaptative = Mat::zeros(distortedFrames[cont].size(), distortedFrames[cont].type());;

	while (cont < numFrames) {
		/* Convert imag e(color) to gray scale (cvtColor) and next to binary (adaptiveThreshold) */
		cvtColor(frames[cont], grayImage, COLOR_BGR2GRAY);
		GaussianBlur(grayImage, grayImage, Size(11, 11), 0, 0);
		adaptiveThreshold(grayImage, binaryAdaptative, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);		// 11 , 4

		bool foundCenters = recognition();

		// If find all inner corners
		if (foundCenters) {
			for (size_t i = 0; i < centers.size(); ++i) centers[i] = rings[i].center;
			//for (size_t i = 0; i < rings.size(); ++i) writeText(frames[cont], rings[i].center, rings[i].id);
			//for (size_t i = 0; i < rings.size() - 1; ++i) line(frames[cont], rings[i].center, rings[i + 1].center, Scalar(255, 255, 0), 1, 8);
			//for (size_t i = 0; i < centers.size(); ++i) circle(frames[cont], centers[i], 2, Scalar(0, 0, 255), 2, 4);
			
			distortCenters.push_back(centers);
		}
		else cout << "NO FIND INITIAL: " << cont << endl;

		imshow("Distorted Initial ", distortedFrames[cont]);

		int key = waitKey(10);
		if (key == 27) return;

		cont++;
	}
}


void RingPattern::colinearity(const vector<vector<Point2f>>& distortCenters)
{
	/* To calculate colinearity error */
	double acumulateHor = 0.0, acumulateVer = 0.0, acumulateTotal = 0.0;

	for (int i = 1; i < numFrames; ++i) {
		for (int j = 1; j < 4; ++j) acumulateHor += distance_to_Line(distortCenters[i][0], distortCenters[i][4], distortCenters[i][j]);
		for (int j = 16; j < 19; ++j) acumulateHor += distance_to_Line(distortCenters[i][15], distortCenters[i][19], distortCenters[i][j]);
		for (int j = 1; j < 3; ++j) acumulateVer += distance_to_Line(distortCenters[i][0], distortCenters[i][15], distortCenters[i][5 *j]);
		for (int j = 2; j < 4; ++j) acumulateVer += distance_to_Line(distortCenters[i][4], distortCenters[i][19], distortCenters[i][j * 5 - 1]);
	}
	acumulateHor /= 6;
	acumulateVer /= 4;

	acumulateTotal += (acumulateHor + acumulateVer) / 2;

	cout << "\nLinearity Error: " << acumulateTotal / numFrames << endl;

}

void RingPattern::calibrationOfCamera(const vector<vector<Point2f>> distCenters)
{
	vector<vector<Point3f>> objectPoints;							// WorldSpace (3D)
	vector<vector<Point2f>> imagePoints;							// Image plane (2D)
	vector<Point3f> grid;

	for (int i = 0; i < VERTICALCENTERSRINGS; ++i) {
		for (int j = 0; j < HORIZONTALCENTERSRINGS; ++j) {
			grid.push_back(Point3f((float)(3 - i), (float)(4 - j), 0.0));
		}
	}

	for (int i = 0; i < numFrames; ++i) {
		imagePoints.push_back(distCenters[i]);
		objectPoints.push_back(grid);
	}

	vector<Mat> rvecs = chessCamera.rotations;									// vectgor of rotations
	vector<Mat> tvecs = chessCamera.translations;

	chessCamera.RMS = calibrateCamera(objectPoints, imagePoints, blackImage.size(), chessCamera.intrinsicParameters, chessCamera.distCoeffs, rvecs, tvecs);

	cout << "RMS # " << iterativeProcess++ << ": " << chessCamera.RMS << endl;
}


vector<vector<Point2f>> RingPattern::recognizeCorners(const vector<Mat>& frames, bool frontoPar)
{
	int cont = 0;
	vector<vector<Point2f>> cornersAll;
	vector<Point2f> centers(HORIZONTALCENTERSRINGS * VERTICALCENTERSRINGS);

	while (cont < numFrames) {
		cvtColor(frames[cont], grayImage, COLOR_BGR2GRAY);
		GaussianBlur(grayImage, grayImage, Size(11, 11), 0, 0);
		if (frontoPar == true) {
			adaptiveThreshold(grayImage, binaryAdaptative, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, 3);
			imshow("Adaptative ", binaryAdaptative);
			waitKey(80);
			//cout << "FP";
		}
		else adaptiveThreshold(grayImage, binaryAdaptative, 205, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 2);
		//imshow("Adaptative ", binaryAdaptative);
		//waitKey(180);
		bool foundCenters = recognition();

		if (foundCenters) {
			for (size_t i = 0; i < centers.size(); ++i) centers[i] = rings[i].center;
			//for (size_t i = 0; i < rings.size(); ++i) writeText(frames[cont], rings[i].center, rings[i].id);
			//for (size_t i = 0; i < rings.size() - 1; ++i) line(frames[cont], rings[i].center, rings[i + 1].center, Scalar(255, 255, 0), 1, 8);
			//for (size_t i = 0; i < centers.size(); ++i) circle(frames[cont], centers[i], 2, Scalar(0, 0, 255), 2, 4);
			cornersAll.push_back(centers);
		}
		else cout << "NOT FOUND: " << cont << endl;

		//imshow("Frame " + to_string(cont), frames[cont]);
		//int key = waitKey(380);

		cont++;
	}
	return cornersAll;
}

void RingPattern::findUndistortedCenters()
{
	undistortedFrames.resize(numFrames);

	for (size_t i = 0; i < numFrames; ++i) {
		undistort(distortedFrames[i], undistortedFrames[i], chessCamera.intrinsicParameters, chessCamera.distCoeffs);
	}

	undistortCenters = recognizeCorners(undistortedFrames, false);					// Fill undistortCenters from undistorted frames

	/* See Undistarted frames 
	for (size_t i = 0; i < numFrames; ++i) {
		//for (size_t j = 0; j < numCircles; ++j) circle(undistortedFrames[i], undistortCenters[i][j], 1, Scalar(0, 0, 255), 1, 8);
		imshow("Undistorted", undistortedFrames[i]);

		int key = waitKey(50);
		if (key == 27) return;
	}
	*/
}


void RingPattern::findFrontoParallelCenters()
{
	// Obtain the frames in fronto parallel
	frontoParallelFrames.resize(numFrames);
	homographies.resize(numFrames);
	vector<Point2f> inputQuad(4), outputQuad(4);
	float offset = 160.0;			 // 160 ->lifecam
	outputQuad[0] = Point2f(0, 0) + Point2f(offset, offset);
	outputQuad[1] = Point2f(grayImage.cols - 1, 0) + Point2f(-offset, offset);
	outputQuad[2] = Point2f(grayImage.cols - 1, grayImage.rows - 1) + Point2f(-offset, -offset);
	outputQuad[3] = Point2f(0, grayImage.rows - 1) + Point2f(offset, -offset);
	
	for (size_t i = 0; i < numFrames; ++i) {
		inputQuad[0] = undistortCenters[i][19]; inputQuad[1] = undistortCenters[i][15];
		inputQuad[2] = undistortCenters[i][0];  inputQuad[3] = undistortCenters[i][4];

		homographies[i] = getPerspectiveTransform(inputQuad, outputQuad);

		warpPerspective(undistortedFrames[i], frontoParallelFrames[i], homographies[i], grayImage.size());
	}

	// Find the centers from fronto parallel images
	frontoParallelCenters = recognizeCorners(frontoParallelFrames, true);

	/* See Undistarted frames 
	for (size_t i = 0; i < numFrames; ++i) {
		//for (size_t j = 0; j < numCircles; ++j) circle(frontoParallelFrames[i], frontoParallelCenters[i][j], 2, Scalar(255, 0, 0), 2, 8);
		imshow("Fronto Parallel", frontoParallelFrames[i]);

		int key = waitKey(200);
		if (key == 27) return;
	}
	*/
}


void RingPattern::findUndistortedCentersReverse()
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


void RingPattern::findDistortedCentersReverse()
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

		int key = waitKey(250);
		if (key == 27) return;
	}
	

	// First method
	for (int i = 0; i < numFrames; ++i){
		for (int j = 0; j < numCircles; ++j){
			distortCenters[i][j] = (distortCenters[i][j] + distortCentersReverse[i][j]) / 2.0;
		}
	}

	// Second method
	//distortCenters.clear();
	//distortCenters = distortCentersReverse;
	//distortCentersReverse.clear();
}


void RingPattern::showUndistortedFinal()
{
	for (size_t i = 0; i < numFrames; ++i) {
		imshow("Undistorted End", undistortedFrames[i]);

		int key = waitKey(100);
		if (key == 27) return;
	}
}

void RingPattern::printFinalDataCamera()
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