#pragma once

#include <string>
#include <opencv2/opencv.hpp>

#include "ArtRobot/Component/Base.h"

namespace ArtRobot
{
namespace Component
{

class Image : public Base
{
private:
      cv::Mat imageMat;

      void drawMat(const cv::Mat &imageMatRead);

public:
      Image(std::string __name,
            double x, double y,
            double w, double h,
            double r,
            const std::string &imageFilePath);
      Image(std::string __name,
            double x, double y,
            double w, double h,
            double r,
            const cv::Mat &imageMatRead);
      ~Image();
};

} // namespace Component
} // namespace ArtRobot
