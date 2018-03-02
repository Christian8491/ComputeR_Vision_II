/* This implementation have 3 steps:
 * 1. Recognition
 * 2. Tracking
 * 3. Calibration Camera
 * 
 * This code was developed by: Christian Córdova & Zoser Quenaya
*/

#include <iostream>
#include <vector>

using namespace std;

/* OpenCV libraries */
#include "opencv2/opencv.hpp"

using namespace cv;

void printData(double& calibration, Mat& intrinsic, Mat& distCoeffs)
{
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

int main()
{
	/* Get values from user: */
	printf("\nCAMERA CALIBRATION: Asymmetric Circle Pattern\n");
	printf("\nDefault values: \n4 rows \n11 cols\n");
	int numSnapshots;
	printf("Enter number of snap shots: ");
	scanf("%d", &numSnapshots);
	printf("\n \n");

	int numCirclesHor = 4, numCirclesVer = 11;
	int cont = 0;

	/* Useful variables */
	int numCircles = numCirclesHor * numCirclesVer;
	Size board_sz = Size(numCirclesHor, numCirclesVer);			// total number of circles

	/* Read a camera or a video */
	// "../../../videos/Calibration/LifeCam/Asymmetric.wmv"
	// "../../../videos/Calibration/Camera_Play_3/Asymmetric.webm"
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cerr << "Error: not read video!" << endl;
		return EXIT_FAILURE;
	}

	/* Create a list of objectPoints (3D) and imagePoints (2D) */
	vector<vector<Point3f>> objectPoints;
	vector<vector<Point2f>> imagePoints;

	vector<Point2f> centers;									// this will be filled by the detected centers
	int successes = 0;

	/* Create two images and get the first snapshot from the camera */
	Mat image, gray_image, blackMat;

	capture >> image;
	blackMat = Mat::zeros(image.size(), image.type());

	vector<Point3f> obj;

	for (int i = 0; i < numCirclesVer; ++i) {
		for (int j = 0; j < numCirclesHor; ++j) {
			obj.push_back(Point3f(2 * j + i % 2, i, 0));
		}
	}

	while (successes < numSnapshots) {
		cvtColor(image, gray_image, CV_BGR2GRAY);
		bool foundCenters = findCirclesGrid(image, board_sz, centers, CALIB_CB_ASYMMETRIC_GRID);

		/* If find all inner corners */
		if (foundCenters) {
			drawChessboardCorners(image, board_sz, centers, foundCenters);
			blackMat = Mat::zeros(image.size(), image.type());
			for (size_t i = 0; i < centers.size(); ++i) circle(blackMat, centers[i], 5, Scalar(0, 0, 255), 2, 8);
		}

		imshow("video", image);
		imshow("2D", blackMat);				// All space 2D

		capture >> image;
		int key = waitKey(1);				// default key = -1

		if (key == 27) return 0;

		if ((cont % 4 == 0) && foundCenters != 0) {
			imagePoints.push_back(centers);
			objectPoints.push_back(obj);
			successes++;

			printf("Snap stored: %d of %d\n", successes, numSnapshots);
			if (successes >= numSnapshots) {
				break;
			}
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

	double calib = calibrateCamera(objectPoints, imagePoints, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
	printData(calib, intrinsic, distCoeffs);

	/* Undistort */
	Mat imageUndistorted;
	while (1) {
		capture >> image;
		undistort(image, imageUndistorted, intrinsic, distCoeffs);

		imshow("Distorted", image);
		imshow("Undistorted", imageUndistorted);
		waitKey(1);
	}

	capture.release();

	return 0;
}