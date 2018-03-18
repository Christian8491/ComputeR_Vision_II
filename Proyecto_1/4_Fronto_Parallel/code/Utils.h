#ifndef UTILS_H
#define UTILS_H

#include "Common.h"

//----------------------------------------------------- Initials ----------------------------------------------------------

#define TOTALRINGS 20

struct Ring
{
	int id;
	Point2f center;

	Ring(){}
};

enum patternType {CHESSBOARD = 1, ASYMMETRIC, RINGS};
enum frameType {TYPEFRAME20 = 1, TYPEFRAME30, TYPEFRAME40, TYPEFRAME50};
enum framesQuantity { TOTALFRAMES20 = 20, TOTALFRAMES30 = 30, TOTALFRAMES40 = 40, TOTALFRAMES50 = 50 };
enum cameraType {LIFECAM = 1, PS3};

//----------------------------------------------------- Functions ---------------------------------------------------------


/** Initialize the parameters according to the user choose */
int initialize(vector<Mat>& frames)
{
	// Get values from user: type of pattern
	printf("\nSelect {1, 2, 3} option for the type of linear pattern:\n");
	printf("1. ChessBoard pattern \n2. Asymmetric pattern \n3. Rings pattern \n");
	int typePattern;
	scanf("%d", &typePattern);
	while (typePattern < 1 || typePattern > 3)
	{
		printf("\nSelect a correct option {1, 2, 3} :\n");
		scanf("%d", &typePattern);
	}

	// Get values from user: number of images
	printf("\nSelect {1, 2, 3, 4} option for the cantity of frames:\n");
	printf("1. 20 frames \n2. 30 frames \n3. 40 frames \n4. 50 frames \n");
	int totalFrames;
	scanf("%d", &totalFrames);
	while (totalFrames < 1 || totalFrames > 4)
	{
		printf("\nSelect a correct option {1, 2, 3, 4} :\n");
		scanf("%d", &totalFrames);
	}

	// Get values from user: type of camera (PS3: Play Station 3 camera)
	printf("\nSelect {1, 2} option for the type of camera:\n");
	printf("1. LifeCam \n2. PS3 \n");
	int typeCamera;
	scanf("%d", &typeCamera);
	while (typeCamera < 1 || typeCamera > 2)
	{
		printf("\nSelect a correct option {1, 2} :\n");
		scanf("%d", &typeCamera);
	}

	// Filling the images
	// ChessBoard
	if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME20 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES20);
		for (size_t i = 0; i < TOTALFRAMES20; ++i) {
			frames[i] = imread("images/ChessBoard/20_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME20 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES20);
		for (size_t i = 0; i < TOTALFRAMES20; ++i) {
			frames[i] = imread("images/ChessBoard/20_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME30 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES30);
		for (size_t i = 0; i < TOTALFRAMES30; ++i) {
			frames[i] = imread("images/ChessBoard/30_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME30 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES30);
		for (size_t i = 0; i < TOTALFRAMES30; ++i) {
			frames[i] = imread("images/ChessBoard/30_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME40 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES40);
		for (size_t i = 0; i < TOTALFRAMES40; ++i) {
			frames[i] = imread("images/ChessBoard/40_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME40 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES40);
		for (size_t i = 0; i < TOTALFRAMES40; ++i) {
			frames[i] = imread("images/ChessBoard/40_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME50 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES50);
		for (size_t i = 0; i < TOTALFRAMES50; ++i) {
			frames[i] = imread("images/ChessBoard/50_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == CHESSBOARD && totalFrames == TYPEFRAME50 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES50);
		for (size_t i = 0; i < TOTALFRAMES50; ++i) {
			frames[i] = imread("images/ChessBoard/50_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}

	// Asymmetric
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME20 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES20);
		for (size_t i = 0; i < TOTALFRAMES20; ++i) {
			frames[i] = imread("images/Asymmetric/20_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME20 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES20);
		for (size_t i = 0; i < TOTALFRAMES20; ++i) {
			frames[i] = imread("images/Asymmetric/20_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME30 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES30);
		for (size_t i = 0; i < TOTALFRAMES30; ++i) {
			frames[i] = imread("images/Asymmetric/30_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME30 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES30);
		for (size_t i = 0; i < TOTALFRAMES30; ++i) {
			frames[i] = imread("images/Asymmetric/30_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME40 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES40);
		for (size_t i = 0; i < TOTALFRAMES40; ++i) {
			frames[i] = imread("images/Asymmetric/40_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME40 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES40);
		for (size_t i = 0; i < TOTALFRAMES40; ++i) {
			frames[i] = imread("images/Asymmetric/40_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME50 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES50);
		for (size_t i = 0; i < TOTALFRAMES50; ++i) {
			frames[i] = imread("images/Asymmetric/50_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == ASYMMETRIC && totalFrames == TYPEFRAME50 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES50);
		for (size_t i = 0; i < TOTALFRAMES50; ++i) {
			frames[i] = imread("images/Asymmetric/50_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}

	// Rings
	else if (typePattern == RINGS && totalFrames == TYPEFRAME20 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES20);
		for (size_t i = 0; i < TOTALFRAMES20; ++i) {
			frames[i] = imread("images/Rings/20_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME20 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES20);
		for (size_t i = 0; i < TOTALFRAMES20; ++i) {
			frames[i] = imread("images/Rings/20_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME30 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES30);
		for (size_t i = 0; i < TOTALFRAMES30; ++i) {
			frames[i] = imread("images/Rings/30_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME30 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES30);
		for (size_t i = 0; i < TOTALFRAMES30; ++i) {
			frames[i] = imread("images/Rings/30_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME40 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES40);
		for (size_t i = 0; i < TOTALFRAMES40; ++i) {
			frames[i] = imread("images/Rings/40_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME40 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES40);
		for (size_t i = 0; i < TOTALFRAMES40; ++i) {
			frames[i] = imread("images/Rings/40_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME50 && typeCamera == LIFECAM) {
		frames.resize(TOTALFRAMES50);
		for (size_t i = 0; i < TOTALFRAMES50; ++i) {
			frames[i] = imread("images/Rings/50_images/LifeCam/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}
	else if (typePattern == RINGS && totalFrames == TYPEFRAME50 && typeCamera == PS3) {
		frames.resize(TOTALFRAMES50);
		for (size_t i = 0; i < TOTALFRAMES50; ++i) {
			frames[i] = imread("images/Rings/50_images/PS3/distorted_" + to_string(i) + ".jpg", CV_LOAD_IMAGE_COLOR);
		}
	}

	return typePattern;
}


/** Calculate the Euclidean distance of two points in 2D (pos1 & pos2)
	@param pos1: position of the point 1
	@param pos2: position of the point 2
	@return distance between the points
*/
inline double euclidian(Point2f pos1, Point2f pos2)
{
	return sqrtf(powf(pos1.x - pos2.x, 2) + powf(pos1.y - pos2.y, 2));
}


/** Calculate the Euclidean distance between a line and a point in 2D space 
	@param startLine: initial position of line
	@param endLine: final position of the line
	@param point: arbitrary point
	@return distance between the point to line 
*/
inline double distance_to_Line(Point2f startLine, Point2f endLine, Point2f point)
{
	double normalLength = _hypot(endLine.x - startLine.x, endLine.y - startLine.y);
	double distance = (double)((point.x - startLine.x) * (endLine.y - startLine.y) - (point.y - startLine.y) * (endLine.x - startLine.x)) / normalLength;
	return abs(distance);
}


/** Put text into the image
	@param image: entry image, where the information will be drawn
	@param center: position where the information will be drawn
	@param id: information to show
*/
inline void writeText(Mat image, Point2f& center, int& id)
{
	putText(image, to_string(id), center - Point2f(15, 15), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 0, 255), 1, 8);
}

/*
double distance_to_Line(Point2f startLine, Point2f endLine, Point2f point)
{
	double normalLength = _hypot(endLine.x - startLine.x, endLine.y - startLine.y);
	double distance = (double)((point.x - startLine.x) * (endLine.y - startLine.y) - (point.y - startLine.y) * (endLine.x - startLine.x)) / normalLength;
	return abs(distance);
}
*/
void printCameraData(double& calibration, Mat& intrinsic, Mat& distCoeffs, vector<Mat>& rvecs, vector<Mat>& tvecs)
{
	int sizeRvecs = rvecs.size(), sizeTvecs = tvecs.size();
	cout << "\nData: " << endl;
	cout << "Calibration Camera Value - RMS: " << calibration << endl;
	cout << "\ndistortion coefficients [k1 k2 p1 p2 k3]: " << endl;
	cout << distCoeffs << endl;
	cout << endl;
	cout << "\nIntrinsic Values:" << endl;
	cout << intrinsic << endl;
	cout << endl;
	/*
	cout << "\nrvecs ->size: " << sizeRvecs << endl;
	for (int i = 0; i < sizeRvecs; ++i) cout << rvecs[i] << endl;
	cout << endl;
	cout << "\ntvecs -> size: " << sizeTvecs << endl;
	for (int i = 0; i < sizeTvecs; ++i) cout << tvecs[i] << endl;
	*/
}

#endif