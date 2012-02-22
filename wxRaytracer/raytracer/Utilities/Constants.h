#ifndef __CONSTANTS__
#define __CONSTANTS__

// 	Copyright (C) Kevin Suffern 2000-2012.
// 	Copyright (C) Dan Rosser 2011-2012.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include <stdlib.h>			// for RAND_MAX
#include "RGBColor.h"

#define	PI 			3.14159265358979323846f
#define TWO_PI 		6.28318530717958647692f
#define INV_PI 		0.31830988618379067154f
#define	INV_TWO_PI 	0.15915494309189533577f
#define	PI_ON_180 	0.0174532925199432957f
#define	HUGE_VALUE	3.402823466e+38f

const double EPSILON 		= 0.001;


// random Consts

const float		INV_RAND_MAX = 1.0 / (float)RAND_MAX;
const float		MT_RAND_MAX = 4294967296;
const float		INV_MT_RAND_MAX = 1.0 / (float)4294967296;

const double	DEG2RAD = PI / 180.0;
const double	RAD2DEG = 180.0 / PI;


// colors

const RGBColor	black(0.0);
const RGBColor	white(1.0);
const RGBColor	red(1.0, 0.0, 0.0);
const RGBColor	green(0.0, 1.0, 0.0);
const RGBColor	blue(0.0, 0.0, 1.0);
const RGBColor	yellow(1.0, 1.0, 0.0);
const RGBColor	cyan(0.0, 1.0, 1.0);
const RGBColor	majenta(1.0, 0.0, 1.0);


// legacy constants 

const double 	invPI 		= 0.31830988618379067154;
const double 	invTWO_PI 	= 0.15915494309189533577;
const double 	kEpsilon 	= 0.0001; 
const double	kHugeValue	= 3.402823466e+38;


#endif
