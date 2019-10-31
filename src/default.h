#ifndef DEFAULT_H
#define DEFAULT_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstring>
#ifdef TIMER
#include <sys/time.h>
#endif // TIMER

#include <cairo.h>
#include <cairo-pdf.h>
//#include <cairo-ps.h>
#include <cairo-svg.h>
#include <cairo-ft.h>
#include <librsvg/rsvg.h>
#include "3rdParty/json.hpp"

using namespace std;
using Json = nlohmann::json;

enum outputType
{
    outputTypeUnknow = 0,
    outputTypeSvg,
    outputTypePdf,
    outputTypePng,
};

#endif // DEFAULT_H
