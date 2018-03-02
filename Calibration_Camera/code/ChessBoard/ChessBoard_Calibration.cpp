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

void printData(double calibration, Mat intrinsic, Mat distCoeffs)
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
}

int main()
{
	/* Get values from user: */
	printf("\nCAMERA CALIBRATION: Chess Board pattern\n");
	printf("\nDefault 9 corners horizontal and 6 vertical corners. \n");
	int numSnapshots;
	printf("Enter number of snap shots: ");
	scanf("%d", &numSnapshots);

	int numCornersHor = 9, numCornersVer = 6;
	int cont = 0;

	/* Useful variables */
	int numSquares = numCornersHor * numCornersVer;
	Size board_sz = Size(numCornersHor, numCornersVer);			// interior number of corners

	/* Read a camera or a video */
	// "../../../videos/Calibration/LifeCam/ChessBoard.vmw"
	// "../../../videos/Calibration/Camera_Play_3/ChessBoard.webm"
	VideoCapture capture("../../../videos/Calibration/Camera_Play_3/ChessBoard.webm");
	if (!capture.isOpened()) {
		cerr << "Error: not read video!" << endl;
		return EXIT_FAILURE;
	}

	/* Create a list of object_points (3D) and image_points (2D) */
	vector<vector<Point3f>> object_points;
	vector<vector<Point2f>> image_points;

	vector<Point2f> corners;									// this will be filled by the detected corners
	int successes = 0;

	/* Create two images and get the first snapshot from the camera */
	Mat image, gray_image, blackMat;

	capture >> image;
	blackMat = Mat::zeros(image.size(), image.type());

	vector<Point3f> obj;
	for (int j = 0; j < numSquares; j++)
		obj.push_back(Point3f(j / numCornersHor, j%numCornersHor, 0.0f));

	while (successes < numSnapshots) {
		cvtColor(image, gray_image, CV_BGR2GRAY);
		bool foundCorners = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		/* If find all inner corners */
		if (foundCorners) {
			cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray_image, board_sz, corners, foundCorners);
			blackMat = Mat::zeros(image.size(), image.type());
			for (size_t i = 0; i < corners.size(); ++i) circle(blackMat, corners[i], 5, Scalar(0, 0, 255), 2, 8);
		}

		imshow("video", gray_image);
		imshow("2D", blackMat);				// All space 2D

		capture >> image;
		int key = waitKey(1);				// default key = -1

		if (key == 27) return 0;

		if (key == ' ' && foundCorners != 0) {
			image_points.push_back(corners);
			object_points.push_back(obj);
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

	double calib = calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

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