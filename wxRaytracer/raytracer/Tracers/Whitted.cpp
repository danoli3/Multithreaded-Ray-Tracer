// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Whitted.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

Whitted::Whitted(void)
	: Tracer()
{}


// -------------------------------------------------------------------- constructor
		
Whitted::Whitted(World* _worldPtr)
	: Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

Whitted::~Whitted(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor	
Whitted::trace_ray(const Ray ray, const int depth) const {
	if (depth > world_ptr->vp.max_depth)
		return(black);
	else {
		ShadeRec sr(world_ptr->hit_objects(ray));    
					
		if (sr.hit_an_object) {
			sr.depth = depth;
			sr.ray = ray;	
			return (sr.material_ptr->shade(sr));   
		}
		else
			return (world_ptr->background_color);
	}																																			
}

RGBColor	
Whitted::trace_ray(const Ray ray, const int depth, int &count, int &jump) const {
	if (depth > world_ptr->vp.max_depth)
		return(black);
	else {
		ShadeRec sr(world_ptr->hit_objects(ray));
		sr.count = &count;
		sr.jump = &jump;
		if (sr.hit_an_object) {
			sr.depth = depth;
			sr.ray = ray;	
			return (sr.material_ptr->shade(sr));   
		}
		else
			return (world_ptr->background_color);
	}																																			
}
