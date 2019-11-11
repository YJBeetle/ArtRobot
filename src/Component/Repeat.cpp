#include "ArtRobot/Component/Repeat.h"

namespace ArtRobot
{
namespace Component
{

Repeat::Repeat(std::string __name,
               double __x, double __y,
               double __w, double __h,
               double __r) // TODO
    : Base(TypeRepeat, __name, __x, __y, __w, __h, __r)
{
}

Repeat::~Repeat()
{
    finish();
}

} // namespace Component
} // namespace ArtRobot
