#pragma once

#include <memory>

using namespace std;

#include "ArtRobot/Component/Base.h"
#include "ArtRobot/Component/Image.h"

namespace ArtRobot
{
namespace Component
{

class ImageMask : public Base
{
private:
shared_ptr<Base> child;

public:
    ImageMask(double x, double y,
              double w, double h,
              double r,
              const string &maskImageFilePath,
              shared_ptr<Base> child);
    ImageMask(double x, double y,
              double w, double h,
              double r,
              const Mat &maskImageMatRead,
              shared_ptr<Base> child);
    ~ImageMask();
};

} // namespace Component
} // namespace ArtRobot
