#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstring>
#ifdef TIMER
#include <sys/time.h>
#endif // TIMER

#include "3rdParty/json.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using Json = nlohmann::json;
using namespace cv;
