#ifndef DRAW_H
#define DRAW_H

#define MM2PT(mm) (mm*(1/25.4)*72)

#define A4_WIDTH  210
#define A4_HEIGHT 297


void drawsvg (cairo_t *cr, char *svgfilename, unsigned int width, unsigned int height);

void drawtext (cairo_t *cr,char *text);

void draw (cairo_surface_t *surface);

void travel_path (cairo_t *cr);

#endif // DRAW_H
