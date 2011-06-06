#ifndef __MULTITHREAD_H__
#define __MULTITHREAD_H__

#include "Point2D.h"
#include "RGBColor.h"

struct RenderedPixel {
  Point2D xy;
  RGBColor color;
} ;

struct RenderedInt {
  int x, y, r, g, b;
} ;

struct PixelPoints {
  Point2D origin;
  Point2D end;  
} ;


#endif
