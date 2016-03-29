#include <stdio.h>
#include <stdint.h>

#include "default.h"
#include "color.h"

color::color()
{
    set();
}

color::color(int32_t code)
{
    this->set(code);
}

color::color(const char *code)
{
    this->set(code);
}

color::color(int8_t red, int8_t green, int8_t blue, int8_t alpha)
{

    this->set(red,green,blue,alpha);
}

color::color(double red, double green, double blue, double alpha)
{
    this->set(red,green,blue,alpha);
}

void color::set()
{
    this->red=0;
    this->green=0;
    this->blue=0;
    this->alpha=0;
}

void color::set(int32_t code)
{
    if(code<=0xffffff)
        this->alpha=(double)1;
    else
        this->alpha=(double)(code>>24&0xff)/(double)0xff;
    this->red=(double)(code>>16&0xff)/(double)0xff;
    this->green=(double)(code>>8&0xff)/(double)0xff;
    this->blue=(double)(code&0xff)/(double)0xff;
}

void color::set(const char *code)
{
    int32_t ecode=0;
    if(code)
    {
        if(code[0]=='#')
            sscanf(code,"#%x",&ecode);
        else if(code[0]=='0'&&code[1]=='x')
            sscanf(code,"0x%x",&ecode);
        else if(code[0]=='0'&&code[1]=='X')
            sscanf(code,"0X%x",&ecode);
        else
            sscanf(code,"%x",&ecode);
    }
    this->set(ecode);
}

void color::set(int8_t red, int8_t green, int8_t blue, int8_t alpha)
{
    this->red=(double)red/(double)0xff;
    this->green=(double)green/(double)0xff;
    this->blue=(double)blue/(double)0xff;
    this->alpha=(double)alpha/(double)0xff;
}

void color::set(double red, double green, double blue, double alpha)
{
    this->red=(red<1)?((red>0)?red:0):1;
    this->green=(green<1)?((green>0)?green:0):1;
    this->blue=(blue<1)?((blue>0)?blue:0):1;
    this->alpha=(alpha<1)?((alpha>0)?alpha:0):1;
}

double color::red_double()
{
    return this->red;
}

double color::green_double()
{
    return this->green;
}

double color::blue_double()
{
    return this->blue;
}

double color::alpha_double()
{
    return this->alpha;
}

