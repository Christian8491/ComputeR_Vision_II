/* This implementation have 3 steps:
 * 1. Recognition
 * 2. Tracking
 * 3. Calibration Camera
 * 
 * This code was developed by: Christian Córdova & Zoser Quenaya
*/

#include <iostream>
#include <vector>

/* OpenCV libraries */
#include "opencv2/opencv.hpp"
using namespace std;

using namespace cv;

struct Ring
{
	Point2f center;
	Scalar color;
	Point2f centerBefore;

	Ring(){}
};

//--------------------------------------------- Initials -------------------------------------------------------

Mat frame, imageGray, binaryAdaptative;
int thresh = 60, framesCounter = 0;
int key_pressed = 0;
bool match = false;
vector<Ring> rings;
vector<Point2f> twentyCenters;

//------------------------------------------ See Camera's Data -------------------------------------------------

void printData(double& calibration, Mat& intrinsic, Mat& distCoeffs)
{
	cout << "SI" << endl;
	cout << "\nData: " << endl;
	cout << "Calibration Camera Value: " << calibration << endl;
	cout << "\ndistCoeffs [k1 k2 p1 p2 k3]: " << endl;
	cout << "k1: " << to_string(distCoeffs.ptr(0)[0]) << endl;
	cout << "k2: " << to_string(distCoeffs.ptr(0)[1]) << endl;
	cout << "p1: " << to_string(distCoeffs.ptr(0)[2]) << endl;
	cout << "p2: " << to_string(distCoeffs.ptr(0)[3]) << endl;
	cout << "k3: " << to_string(distCoeffs.ptr(0)[4]) << endl;
	cout << "\nIntrinsic Values: " << endl;
	cout << "distancel focal x: " << to_string(intrinsic.ptr(0)[0]) << endl;
	cout << "distancel focal y: " << to_string(intrinsic.ptr(1)[1]) << endl;
	cout << "optical center (x, y) = (" << to_string(intrinsic.ptr(0)[2]) << ", " << to_string(intrinsic.ptr(0)[2]) << ")" << endl;
	cout << endl;
}

//------------------------------------------ Principal Function ------------------------------------------------

inline double euclidian(Point2f pos1, Point2f pos2)
{
	return sqrtf(powf(pos1.x - pos2.x, 2) + powf(pos1.y - pos2.y, 2));
}

void fillRings(vector<Point2f>& centers)
{
	for (size_t i = 0; i < rings.size(); ++i) {
		rings[i].center = centers[i];
		rings[i].centerBefore = centers[i];
	}

	/* Only to see tracking */
	rings[0].color = Scalar(255.0, 0.0, 0.0);
	rings[1].color = Scalar(0.0, 255.0, 0.0);
	rings[2].color = Scalar(0.0, 0.0, 255.0);
	rings[3].color = Scalar(255.0, 255.0, 0.0);
	rings[4].color = Scalar(0.0, 255.0, 255.0);
	rings[5].color = Scalar(255.0, 0.0, 255.0);
}


void findRingNearest(vector<Point2f>& centers)
{
	if (centers.size() >= 20) {
		for (size_t i = 0; i < rings.size(); ++i) {
			vector<double> distances(centers.size());
			for (size_t j = 0; j < centers.size(); ++j) {
				distances[j] = euclidian(rings[i].centerBefore, centers[j]);
			}
			int minDistIndex = distance(distances.begin(), min_element(distances.begin(), distances.end()));
			rings[i].center = centers[minDistIndex];
		}

		for (size_t i = 0; i < rings.size(); ++i) {
			rings[i].centerBefore = rings[i].center;
		}
	}
	else
	{
		match = false;
		cout << "Out of boundary!" << endl;
	}
}

bool patternRecognition()
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

	/* Algorithm: OnlyTwentyCenters */
	for (size_t i = 0; i < rectanglesRot.size(); ++i) {
		int contador = 0;
		Point2f auxCenter(0.0, 0.0);
		for (size_t j = i + 1; j < rectanglesRot.size(); ++j) {
			if (euclidian(rectanglesRot[i].center, rectanglesRot[j].center) < 1.0) {
				auxCenter += rectanglesRot[j].center;
				contador++;
			}
		}
		if (contador == 3) twentyCenters.push_back(auxCenter / 3);
	}

	if (match == false) {
		if (twentyCenters.size() == 20) {
			match = true;
			fillRings(twentyCenters);
			cout << "MATCH!" << endl;
		}
	}

	else findRingNearest(twentyCenters);

	if (twentyCenters.size() >= 20) return true;
	return false;
}

//-----------------------------------------------------------------------------------------------------------

inline void writeText(double time)
{
	int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
	double fontScale = 1;
	int thickness = 2;
	Point textPos1(15, 25);
	Point textPos2(15, 50);
	string numberFrame = "Frame: " + to_string(framesCounter++);
	string timeElapsed = "Time: " + to_string(time) + " s";

	putText(frame, numberFrame, textPos1, fontFace, fontScale, Scalar::all(50), thickness, 8);
	putText(frame, timeElapsed, textPos2, fontFace, fontScale, Scalar::all(50), thickness, 8);
}

int main(){

	/* Get values from user: */
	int numSnapshots;
	printf("\nCAMERA CALIBRATION: Rings Pattern\n");
	printf("\nDefault values: \n5 rings by row \n4 rings by col\n");
	printf("Enter number of snap shots: ");
	scanf("%d", &numSnapshots);
	printf("\n \n");

	int numRingsHor = 5, numRingsVer = 4;
	int cont = 0;

	/* Useful variables */
	int numRings = numRingsHor * numRingsVer;
	Size boardSize = Size(numRingsHor * 2, numRingsVer * 2);			// total number of circles

	/* Read a camera or a video */
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cerr << "Error: not read video!" << endl;
		return EXIT_FAILURE;
	}

	/* Create a list of objectPoints (3D) and imagePoints (2D) */
	vector<vector<Point3f>> objectPoints;
	vector<vector<Point2f>> imagePoints;

	int successes = 0;

	/* Create two images and get the first snapshot from the camera */
	Mat blackMat;

	capture >> frame;
	blackMat = Mat::zeros(frame.size(), frame.type());

	vector<Point3f> obj;
	for (int i = 0; i < numRingsVer; ++i) {
		for (int j = 0; j < numRingsHor; ++j) {
			obj.push_back(Point3f((float)j, (float)i, 0.0));
		}
	}
	
	rings.resize(20);

	while (successes < numSnapshots) {
		capture >> frame;

		/* Convert frame (color) to gray scale (cvtColor) and next to binary (adaptiveThreshold) */
		cvtColor(frame, imageGray, COLOR_BGR2GRAY);
		GaussianBlur(imageGray, imageGray, Size(11, 11), 0, 0);
		adaptiveThreshold(imageGray, binaryAdaptative, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);
		bool foundCenters = patternRecognition();

		/* Our implementation */
		/* If find all inner corners */
		if (foundCenters) {
			blackMat = Mat::zeros(frame.size(), frame.type());
			for (int i = 0; i < 5; ++i) circle(frame, rings[i].center, 6, rings[i].color, 2, 8);
			for (size_t i = 0; i < rings.size(); ++i) circle(blackMat, rings[i].center, 6, Scalar(255, 0, 0), 2, 8);
		}

		imshow("video", frame);
		imshow("2D", blackMat);				// All space 2D

		capture >> frame;
		int key = waitKey(1);				// default key = -1

		if (key == 27) return 0;

		if ((cont % 15 == 0) && foundCenters && twentyCenters.size() == 20) {
			imagePoints.push_back(twentyCenters);
			objectPoints.push_back(obj);
			successes++;

			printf("Snap stored: %d of %d\n", successes, numSnapshots);
			if (successes >= numSnapshots) break;
		}
		cont++;
	}

	/* Calibration Camera */
	Mat intrinsic = Mat(3, 3, CV_32FC1);
	Mat distCoeffs;
	vector<Mat> rvecs;									// vectgor of rotations
	vector<Mat> tvecs;									// vector of translation

	intrinsic.ptr<float>(0)[0] = 1;						// focal lenght in X
	intrinsic.ptr<float>(1)[1] = 1;						// focal lenght in Y

	if (objectPoints.size() == imagePoints.size()) {
		double calib = calibrateCamera(objectPoints, imagePoints, frame.size(), intrinsic, distCoeffs, rvecs, tvecs);
		printData(calib, intrinsic, distCoeffs);
	}
	

	/* Undistort */
	Mat imageUndistorted;
	while (1) {
		capture >> frame;
		undistort(frame, imageUndistorted, intrinsic, distCoeffs);

		imshow("Distorted", frame);
		imshow("Undistorted", imageUndistorted);
		waitKey(1);
	}

	capture.release();

	return 0;
}