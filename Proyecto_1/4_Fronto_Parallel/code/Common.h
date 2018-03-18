#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip> 

using namespace std;

/* OpenCV libraries */
#include "opencv2/opencv.hpp"

using namespace cv;

using namespace std::chrono;

#define TOTALRINGS 20

/* Active(Debug)
$(OPENCV_DIR)\..\..\include					-> C/C++ General -> Directorios de inclusión adicionales
$(OPENCV_DIR)\lib							-> Vinculador/general -> Directorios de bibliotecas adicionales
opencv_ts300d.lib;opencv_world300d.lib;		-> Vinculador/entrada -> Dependencias adicionales
*/

#endif