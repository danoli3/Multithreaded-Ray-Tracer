#ifndef __MULTITHREAD_H__
#define __MULTITHREAD_H__

//  Copyright (C) 2011 Daniel Rosser
//	This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program. See /LICENSES/gpl-3.0.txt
//    If not, see <http://www.gnu.org/licenses/>.
//
//    Daniel Rosser <danoli3@gmail.com> 

#include "Point2D.h"
#include "RGBColor.h"

struct RenderedPixel {
  Point2D xy;
  RGBColor color;
} ;

struct RenderedInt {
  int x, y, r, g, b;
} ;

struct Pixel
{
public:
   Pixel(unsigned short xi, unsigned short yi) : x(xi), y(yi)
   { }

   unsigned short x, y;
};


enum RenderDisplay { EVERY_PIXEL, EVERY_ROW, EVERY_JOB };


#endif
