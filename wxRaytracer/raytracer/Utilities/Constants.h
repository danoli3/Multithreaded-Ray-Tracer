#ifndef __CONSTANTS__
#define __CONSTANTS__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include <stdlib.h>			// for RAND_MAX
#include "RGBColor.h"

const double 	PI 			= 3.1415926535897932384;
const double 	TWO_PI 		= 6.2831853071795864769;
const double 	PI_ON_180 	= 0.0174532925199432957;
const double 	invPI 		= 0.3183098861837906715;
const double 	invTWO_PI 	= 0.1591549430918953358;

const double	DEG2RAD = PI / 180.0;
const double	RAD2DEG = 180.0 / PI;

const double 	kEpsilon 	= 0.0001; 
const double	kHugeValue	= 1.0E10;

const RGBColor	black(0.0);
const RGBColor	white(1.0);
const RGBColor	red(1.0, 0.0, 0.0);

const float invRAND_MAX = 1.0 / (float)RAND_MAX;
const float MT_RAND_MAX = 4294967296;
const float invMT_RAND_MAX = 1.0 / (float)4294967296;



#endif
