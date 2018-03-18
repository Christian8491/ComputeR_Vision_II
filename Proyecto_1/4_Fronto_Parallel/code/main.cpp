#include "Common.h"
#include "Utils.h"
#include "Camera.h"
#include "ChessboardPattern.h"
#include "AsymmetricPattern.h"
#include "RingPattern.h"
//-------------------------------------------------- Initials -------------------------------------------------------

Mat frameInput, grayImage, binaryAdaptative;
int thresh = 60;
int successes = 0, totalFrames;
bool match = false;

vector<Mat> frames;
vector<Ring> rings(20);
vector<Point2f> twentyCenters;

//----------------------------------------------------- Functions ---------------------------------------------------------


int main()
{
	int typePattern = initialize(frames);
	totalFrames = frames.size();
	
	if (typePattern == CHESSBOARD) {
		ChessboardPattern chessPattern;

		chessPattern.recognizeCornersFirstTime(frames);
		chessPattern.colinearity(chessPattern.distortCenters);
		for (int i = 0; i < 10; ++i) {
			chessPattern.calibrationOfCamera(chessPattern.distortCenters);
			chessPattern.findUndistortedCenters();
			chessPattern.findFrontoParallelCenters();
			chessPattern.findUndistortedCentersReverse();
			chessPattern.findDistortedCentersReverse();
			chessPattern.colinearity(chessPattern.undistortCenters);
		}
		chessPattern.printFinalDataCamera();
		chessPattern.colinearity(chessPattern.undistortCenters);
		chessPattern.showUndistortedFinal();
		chessPattern.calibrationOfCamera(chessPattern.distortCenters);
		//chessPattern.colinearityError();
	}
	else if (typePattern == ASYMMETRIC) {
		AsymmetricPattern asymmetricPattern;

		asymmetricPattern.recognizeCornersFirstTime(frames);
		asymmetricPattern.colinearity(asymmetricPattern.distortCenters);
		
		for (int i = 0; i < 10; ++i) {
			asymmetricPattern.calibrationOfCamera(asymmetricPattern.distortCenters);
			asymmetricPattern.findUndistortedCenters();
			asymmetricPattern.findFrontoParallelCenters();
			asymmetricPattern.findUndistortedCentersReverse();
			asymmetricPattern.findDistortedCentersReverse();
			asymmetricPattern.colinearity(asymmetricPattern.undistortCenters);
		}
		
		asymmetricPattern.showUndistortedFinal();
		asymmetricPattern.printFinalDataCamera();
		asymmetricPattern.colinearity(asymmetricPattern.undistortCenters);
		asymmetricPattern.calibrationOfCamera(asymmetricPattern.distortCenters);
		

	}
	else if (typePattern == RINGS) {
		RingPattern ringPattern;

		ringPattern.recognizeCornersFirstTime(frames);
		ringPattern.colinearity(ringPattern.distortCenters);

		//for (int i = 0; i < 3; ++i) {
			ringPattern.calibrationOfCamera(ringPattern.distortCenters);
			cout << "A";
			ringPattern.findUndistortedCenters();
			cout << "B";
			ringPattern.findFrontoParallelCenters();
			cout << "C";
			ringPattern.findUndistortedCentersReverse();
			cout << "D";
			ringPattern.findDistortedCentersReverse();
			cout << "E";
			ringPattern.colinearity(ringPattern.undistortCenters);
		//}
		ringPattern.showUndistortedFinal();
		ringPattern.printFinalDataCamera();
		ringPattern.calibrationOfCamera(ringPattern.distortCenters);
		ringPattern.calibrationOfCamera(ringPattern.undistortCenters);
	}
	return 0;
}

