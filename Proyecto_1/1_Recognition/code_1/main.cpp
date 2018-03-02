#include "common.h"

Mat src_gray;
int thresh = 100;
RNG rng(12345);
vector<Point2f> centers;
Mat drawing;
int totalEllipses;

vector<double> firstCenters = { 1230.5, 628,
								1104.5, 624.5,
								980, 621,
								856.5, 618.5,
								735.5, 615,
								1233.5, 502,
								1107, 499.5,
								982.5, 497,
								859.25, 494.25,
								738, 492,
								1237, 375.5,
								1110.5, 374,
								985.038, 371.808,
								861.5, 370.5,
								739.5, 368.5,
								1240.75, 247.75,
								1113.5, 246.5,
								987.75, 245.25,
								863.5, 244.5,
								740.5, 243.5 
};

//------------------------------------------------------------------------------------------------------------

/* Divide a video in many frames when 'f' key is pressed */
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

//------------------------------------------------------------------------------------------------------------

/* Convert the frames into black and white (binary) images */
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

//------------------------------------------------------------------------------------------------------------

int thresh_callback(int, void*)
{
	vector<vector<Point> > contours;
	centers.clear();
	
	findContours(src_gray, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));		// Find contours

	vector<RotatedRect> minRect2(contours.size());												// auxiliar minRect
	vector<RotatedRect> minRect;

	for (int i = 0; i < contours.size(); i++) {
		minRect2[i] = minAreaRect(Mat(contours[i]));
	}
	
	/* Find only 20 contours */
	int totalEllipses = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (minRect2[i].size.width > 30 && minRect2[i].size.height > 32 && minRect2[i].size.width < 90 && minRect2[i].size.height < 90 && minRect2[i].center.x < 1700 && minRect2[i].center.x > 300) {
			minRect.push_back(minRect2[i]);
			totalEllipses++;
		}
	}

	cout << totalEllipses << endl;

	/* Fill the center into the centers structure */
	centers.resize(totalEllipses);
	for (int i = 0; i < totalEllipses; i++) centers[i] = minRect[i].center;

	return totalEllipses;
}


/* To FitEllipse */
int findCenters(String path)
{
	src_gray = imread(path, IMREAD_GRAYSCALE);
	return thresh_callback(0, 0);
}

//------------------------------------------------------------------------------------------------------------

/* Show many frames consecutively  */
void showFramesConsecutively(String path, int totalEllipses)
{
	Mat frames = imread(path, IMREAD_COLOR);

	for (int i = 0; i < totalEllipses - 1; i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		line(frames, cvPoint(centers[i].x, centers[i].y), cvPoint(centers[i + 1].x, centers[i + 1].y), color, 1, CV_AA);
		circle(frames, cvPoint(centers[i].x, centers[i].y), 10, color, 1, 8);
	}

	/* Showing image */
	namedWindow("Initial Image", WINDOW_AUTOSIZE);
	imshow("Initial Image", frames);
	waitKey(33);
}

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
	int sum, s = 1/8, t = 15;

	for (int i = 0; i < width; i++) {
		sum = 0;
		for (int j = 0; j < height; j++) {
			sum = sum + inImage.data[i,j];
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


int main()
{	
	/* Only to read the video and convert to frames */
	//const char* videoName = "videos/Kinect2_rgb.avi";
	//divideToFrames(videoName);

	int frameMin = 1, frameMax = 1757;
	vector<String> frameNames;
	String aux;

	for (int i = 0; i < frameMax; i++) {
		aux = "frame_" + to_string(i + 1);
		frameNames.push_back(aux);
	}

	for (int i = 0; i < frameMax; i++) {
		int kk = findCenters("binary_images/" + frameNames[i] + "_binary.png");
		showFramesConsecutively("frames/" + frameNames[i] + ".png", kk);
	}
	

	return 0;
}