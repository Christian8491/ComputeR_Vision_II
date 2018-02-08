#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <random>

using namespace std;


/* OpenCV libraries */
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>							//  input and output operations
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>
#include <opencv/cxcore.h>
#include <fstream>

using namespace cv;

/* 
Active(Debug)

$(OPENCV_DIR)\..\..\include    -> C/C++ General -> Directorios de inclusión adicionales

$(OPENCV_DIR)\lib				-> Vinculador/general -> Directorios de bibliotecas adicionales

opencv_ts300d.lib;opencv_world300d.lib;		-> Vinculador/entrada -> Dependencias adicionales

*/

#endif