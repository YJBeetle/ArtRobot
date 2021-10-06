/*
 * Copyright 2020 YJBeetle
 *
 * Authors:
 *  YJBeetle <YJBeetle@gmail.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 3.  See
 * the LICENSE file in the top-level directory.
 *
 */

#pragma once

#include <string>
#include <opencv2/opencv.hpp>

#include "./Base.hpp"

namespace ArtRobot
{
namespace Component
{

class Image : public Base
{
private:
      void drawMat(const cv::Mat &imageMatRead);

public:
      Image(std::string __name,
            double x, double y,
            double w, double h,
            double r);
      Image(std::string __name,
            double x, double y,
            double w, double h,
            double r,
            cairo_surface_t * imageSurface);
      static std::shared_ptr<Image> fromRaw(std::string __name,
                                            double x, double y,
                                            double w, double h,
                                            double r,
                                            unsigned char * imageData,
                                            int imageW, int imageH,
                                            int imageStride,
                                            char imagePixelBits,
                                            bool premultipliedAlpha);
      static std::shared_ptr<Image> fromMat(std::string __name,
                                            double x, double y,
                                            double w, double h,
                                            double r,
                                            const cv::Mat &imageMat);
      static std::shared_ptr<Image> fromFileByCV(std::string __name,
                                                 double x, double y,
                                                 double w, double h,
                                                 double r,
                                                 const std::string &imageFilePath);
      static std::shared_ptr<Image> fromPNG(std::string __name,
                                            double x, double y,
                                            double w, double h,
                                            double r,
                                            const std::string &imageFilePath);
      static std::shared_ptr<Image> fromJPG(std::string __name,
                                            double x, double y,
                                            double w, double h,
                                            double r,
                                            const std::string &imageFilePath);
      static std::shared_ptr<Image> fromFile(std::string __name,
                                             double x, double y,
                                             double w, double h,
                                             double r,
                                             const std::string &imageFilePath);
      ~Image();
};

} // namespace Component
} // namespace ArtRobot
