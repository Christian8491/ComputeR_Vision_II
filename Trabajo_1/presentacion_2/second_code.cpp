#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace cv;

//--------------------------------------------  Initials  ----------------------------------------------

Mat frame, src_gray;
int thresh = 60, max_thresh = 255;

//------------------------------------------------------------------------------------------------------------

/*  Convert an RGB image to black and white (binary) image using INTEGRAL IMAGE -- NOT FOUND!!! */
void adaptativeThreshold(String nameFile)
{
	Mat inImage = imread("frames/" + nameFile + ".png", IMREAD_COLOR);

	if (inImage.empty()) {
		cout << "Image: " << nameFile << ".png  Not read" << endl;
		return;
	}

	Mat outImage(inImage.size(), inImage.type());

	Mat intImg(inImage.size(), inImage.type());
	intImg.setTo(Scalar::all(0));

	int width = inImage.cols;
	int height = inImage.rows;
	int sum, s = 1 / 8, t = 15;

	for (int i = 0; i < width; i++) {
		sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + inImage.data[i, j];
			if (i == 0) intImg.data[i, j] = sum;
			else intImg.data[i, j] = intImg.data[i, j] + sum;
		}
	}

	int x1, x2, y1, y2;
	int count;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			x1 = i - s / 2;
			x2 = i + s / 2;
			y1 = j - s / 2;
			y2 = j + s / 2;
			count = (x2 - x1) * (y2 - y1);
			sum = intImg.data[x2, y2] - intImg.data[x2, y1 - 1] - intImg.data[x1 - 1, y2] + intImg.data[x1 - 1, y1 - 1];
			if (inImage.data[i, j] * count <= sum * (100 - t) / 100) outImage.data[i, j] = 0;
			else outImage.data[i, j] = 255;
		}
	}

	namedWindow("Binary Image", CV_WINDOW_AUTOSIZE);
	imshow("Binary Image", outImage);

	/* Save the image and wait untill user exits the program */

	const String auxPath = "Integral_Image/" + nameFile + "_integral.png";
	imwrite(auxPath, outImage);
	waitKey(0);
}

//------------------------------------------------------------------------------------------------------------

/* Divide a video in many frames (each 33 ms) */
void divideToFrames(const char* videoName)
{
	CvCapture* capture = cvCreateFileCapture(videoName);				// videos/PadronCirculos_01.avi

	if (!capture) {
		cerr << "The video was not opened!" << endl;
		return;
	}

	int num = 0;
	IplImage* frame;
	char s[20];

	while (1) {
		num++;
		frame = cvQueryFrame(capture);
		if (!frame) break;
		cvShowImage("DisplayVideo", frame);
		char c = cvWaitKey(33);
		if (c == 27) break;
		sprintf(s, "frames/frame_%d.png", num);
		cvSaveImage(s, frame);
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("DisplayVideo");
}

//-----------------------------------------------------------------------------------------------------------

/* Convert the frames into binary images with CLASSICAL METHOD */
void toBinaryImage()
{
	int frameMin = 1, frameMax = 1757;
	vector<String> frameNames;
	String aux;

	for (int i = 0; i < frameMax; i++) {
		aux = "frame_" + to_string(i + 1);
		frameNames.push_back(aux);
	}

	Mat img_src, img_gray;

	for (int i = 0; i < frameMax; i++) {
		img_src = imread("frames/" + frameNames[i] + ".png", IMREAD_COLOR);			// Read the input image
		cvtColor(img_src, img_gray, CV_BGR2GRAY);									// Convert to grayscale

		Mat binaryMat(img_gray.size(), img_gray.type());							// Binary image
		threshold(img_gray, binaryMat, 90, 255, cv::THRESH_BINARY);					// Apply thresholding

		const String auxPath = "binary_images/" + frameNames[i] + "_binary.png";
		imwrite(auxPath, binaryMat);												// save the binary image
	}
}

//-----------------------------------------------------------------------------------------------------------

/* Convert the frames into binary images with ADAPTIVE THRESHOLD */
void adaptiveMeanThreshold()
{
	int frameMin = 1, frameMax = 1757;
	vector<String> frameNames;
	String name;

	for (int i = 0; i < frameMax; i++) {
		name = "frame_" + to_string(i + 1);
		frameNames.push_back(name);
	}

	Mat img_src, img_gray;

	for (int i = 0; i < frameMax; i++) {
		img_src = imread("frames/" + frameNames[i] + ".png", IMREAD_COLOR);			// Read the input image
		cvtColor(img_src, img_gray, CV_BGR2GRAY);									// Convert to grayscale

		Mat adaptativeImg(img_gray.size(), img_gray.type());
		adaptiveThreshold(img_gray, adaptativeImg, 4000.0, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 187, 0); 	// Binary image (adaptive)

		const String auxPath = "adaptative/" + frameNames[i] + "_binary.png";
		imwrite(auxPath, adaptativeImg);												// save the binary image
	}
}

float euclidian(Point2f pos1, Point2f pos2)
{
	return sqrtf(powf(pos1.x - pos2.x, 2) + powf(pos1.y - pos2.y, 2));
}

void thresh_callback(int, void*){

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<RotatedRect> minEllipse;

	Canny(src_gray, canny_output, thresh, thresh * 2, 3);										// Detect edges using Threshold
	findContours(canny_output, contours, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));		// Find contours 

	for (int i = 0; i < contours.size(); i++){
		if (contours[i].size() > 5) minEllipse.push_back(fitEllipse(contours[i]));
	}

	vector<int> histPoint(minEllipse.size());

	for (int i = 0; i< minEllipse.size(); i++){
		for (int j = 0; j < minEllipse.size(); j++){
			if (euclidian(minEllipse[i].center, minEllipse[j].center) < 3.0) histPoint[i]++;
		}
	}

	vector<RotatedRect> minEllipse2;
	for (int i = 0; i< histPoint.size(); ++i){
		if (histPoint[i] == 4 && minEllipse[i].size.width < 65){
			minEllipse2.push_back(minEllipse[i]);
		}
	}

	/* Draw lines to track */
	int k = minEllipse2.size() / 20;
	if (k > 0) {
		for (int i = 0; i < 19; i++) {
			line(frame, minEllipse2[k * i].center, minEllipse2[k * (i + 1)].center, Scalar(0, 255, 0), 1, CV_AA);
		}
	}

	/* Draw circles to track */
	for (int i = 0; i < minEllipse2.size(); i++) {
		circle(frame, minEllipse2[i].center, 6, Scalar(0, 0, 255), 2, 8);
	}

	imshow("result", frame);
	waitKey(01);
}

//-----------------------------------------------------------------------------------------------------------

int main(){

	/* Read the video */
	VideoCapture cap(0); // "videos/Kinect2_rgb.avi"
	if (!cap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	namedWindow("result", WINDOW_AUTOSIZE);				// Create Window

	for (;;){

		using namespace std::chrono;
		high_resolution_clock::time_point t1 = high_resolution_clock::now();

		cap >> frame;

		/* Convert a frame (color) to gray scale (cvtColor) and next to binary (adaptiveThreshold) */
		cvtColor(frame, src_gray, COLOR_BGR2GRAY);
		GaussianBlur(src_gray, src_gray, Size(11, 11), 0, 0);
		adaptiveThreshold(src_gray, src_gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);

		//createTrackbar(" canny:", "result", &thresh, max_thresh, thresh_callback);
		thresh_callback(0, 0);

		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
		//std::cout << "It took me " << time_span.count() << " seconds." << endl;

	}

	return 0;
}