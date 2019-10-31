#ifndef COLOR_H
#define COLOR_H

class Color
{
private:
    uint32_t color;

public:
    Color();
    Color(uint32_t __color);
    Color(const char *__color);
    Color &operator=(uint32_t __color);
    Color &operator=(const char *__color);
    Color &operator=(const Color &__color);
    double red();
    double green();
    double blue();
    double alpha();
};

#endif // COLOR_H
