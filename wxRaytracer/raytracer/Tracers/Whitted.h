#ifndef __WHITTED__
#define __WHITTED__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Tracer.h"

class Whitted: public Tracer {
	public:
		
		Whitted(void);
		
		Whitted(World* _worldPtr);
		
		virtual											
		~Whitted(void);
	
		virtual RGBColor	
		trace_ray(const Ray ray, const int depth) const;

		virtual RGBColor	
		trace_ray(const Ray ray, const int depth, int &count, int &jump) const;
};

#endif
