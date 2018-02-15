
//g++ -Wall -o patronControlPoints patronControlPoints.cpp `pkg-config --cflags --libs opencv`
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/videoio/videoio.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

Mat frame;
Mat drawing;
Mat src; Mat src_gray;
int thresh = 70;
int max_thresh = 255;

void thresh_callback(int, void* );

int main(){

    //release();
    VideoCapture cap("calibration_kinectv2.avi");
      if(!cap.isOpened()){
             cout << "Error opening video stream or file" << endl;
             return -1;
      }

   int frame_width= cap.get(CV_CAP_PROP_FRAME_WIDTH);
   int frame_height= cap.get(CV_CAP_PROP_FRAME_HEIGHT);
   VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height),true);

   for(;;){

      cap >> frame;
      ///////////////////////////////////////////////////////
      
      /// Convert image to gray and blur it
      cvtColor( frame, src_gray, COLOR_BGR2GRAY );
      blur( src_gray, src_gray, Size(3,3) );

      /// Create Window
      namedWindow( "result", WINDOW_NORMAL );

      //createTrackbar( " Canny thresh:", "result", &thresh, max_thresh, thresh_callback );
      //thresh_callback( 0, 0 );
      createTrackbar( " Threshold:", "result", &thresh, max_thresh, thresh_callback );
      thresh_callback( 0, 0 );

      /////////////////////////////////////////////////////////
      video.write(drawing);
      //video.write(frame);
      //imshow("result", frame);
      char c = (char)waitKey(33);
      if( c == 27 ) break;
    }
  return 0;
}

void thresh_callback(int, void* ){
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  /// Find contours
  findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect( contours.size() );
  vector<RotatedRect> minEllipse( contours.size() );

  for( size_t i = 0; i < contours.size(); i++ )
     { minRect[i] = minAreaRect( contours[i] );
       if( contours[i].size() > 5 )
         { minEllipse[i] = fitEllipse( contours[i] ); }
     }

  /// Draw contours + rotated rects + ellipses
  drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
     {
       // contour
       drawContours( drawing, contours, (int)i, Scalar::all(255), 1, 8);
       //drawContours( drawing, contours, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       // ellipse
       //ellipse( drawing, minEllipse[i], color, 2, 8 );
       // rotated rectangle
       Point2f rect_points[4]; minRect[i].points( rect_points );
       for( int j = 0; j < 4; j++ )
          line( drawing, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255)/*Scalar::all(255)*/, 1, 8 );
     }

  /// Show in a window
  //namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "result", drawing );
}