#include <stdio.h>
#include <stdint.h>

#include "default.h"
#include "Color.h"


void Color::set()
{
    this->red=0;
    this->green=0;
    this->blue=0;
    this->alpha=0;
}

void Color::set(int32_t code)
{
    if(code<=0xffffff)
        this->alpha=(double)1;
    else
        this->alpha=(double)(code>>24&0xff)/(double)0xff;
    this->red=(double)(code>>16&0xff)/(double)0xff;
    this->green=(double)(code>>8&0xff)/(double)0xff;
    this->blue=(double)(code&0xff)/(double)0xff;
}

void Color::set(const char *code)
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

void Color::set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    this->red=(double)red/(double)0xff;
    this->green=(double)green/(double)0xff;
    this->blue=(double)blue/(double)0xff;
    this->alpha=(double)alpha/(double)0xff;
}

void Color::set(double red, double green, double blue, double alpha)
{
    this->red=(red<1)?((red>0)?red:0):1;
    this->green=(green<1)?((green>0)?green:0):1;
    this->blue=(blue<1)?((blue>0)?blue:0):1;
    this->alpha=(alpha<1)?((alpha>0)?alpha:0):1;
}

