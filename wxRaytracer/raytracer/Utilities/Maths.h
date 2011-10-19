#ifndef __MATHS__
#define __MATHS__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// This is a header file for the mathematics utility functions
// The prototypes of any inlined functions must be placed here

#include <stdlib.h>

#include "Constants.h"
#include "RGBColor.h"
#include "MTRand.h"

//static MTRand_int32 mtrand;
//static MTRand mtrand_double;


// prototypes

double
min(double x0, double x1);

double
max(double x0, double x1);

double
clamp(const double x, const double min, const double max);

int
SolveQuadric(double c[3], double s[2]);

int 
SolveCubic(double c[4], double s[3]);

int 
SolveQuartic(double c[5], double s[4]);


// inlined functions
// ----------------------------------------------------------------- min

inline double
min(double x0, double x1) {
	return ((x0 < x1) ? x0 : x1);
}

// ----------------------------------------------------------------- max

inline double
max(double x0, double x1) {
	return ((x0 > x1) ? x0 : x1);
}

// ---------------------------------------------------- clamp

inline double
clamp(const double x, const double min, const double max) {
	return (x < min ? min : (x > max ? max : x));
}

#endif
