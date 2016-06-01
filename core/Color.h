#ifndef COLOR_H
#define COLOR_H

class Color
{
private:
    double red;
    double green;
    double blue;
    double alpha;

public:
    Color(){ set(); }
    Color(int32_t code){ set(code); }
    Color(const char *code){ set(code); }
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha){ set(red, green, blue, alpha); }
    Color(double red, double green, double blue, double alpha){ set(red, green, blue, alpha); }
    void set();
    void set(int32_t code);
    void set(const char *code);
    void set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    void set(double red, double green, double blue, double alpha);
    double redDouble() { return red; }
    double greenDouble() { return green; }
    double blueDouble() { return blue; }
    double alphaDouble() { return alpha; }
};

#endif // COLOR_H
