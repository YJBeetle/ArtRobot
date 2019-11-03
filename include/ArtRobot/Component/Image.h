#pragma once

#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Image : public Base
{
private:
public:
      Image(double x, double y,
            double w, double h,
            double r,
            const string &imageFilePath);
      Image(double x, double y,
            double w, double h,
            double r,
            const Mat &imageMatRead);
      ~Image();
};

} // namespace Component
} // namespace ArtRobot
