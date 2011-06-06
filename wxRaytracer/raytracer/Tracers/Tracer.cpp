#include "Tracer.h"

// -------------------------------------------------------------------- default constructor

Tracer::Tracer(void)
	: world_ptr(NULL)
{}


// -------------------------------------------------------------------- constructor

Tracer::Tracer(World* _worldPtr)
	: world_ptr(_worldPtr)
{}



// -------------------------------------------------------------------- destructor

Tracer::~Tracer(void) {
	if (world_ptr)
		world_ptr = NULL;
}


// -------------------------------------------------------------------- trace_ray

RGBColor	
Tracer::trace_ray(const Ray& ray) const {
	return (black);
}


// -------------------------------------------------------------------- trace_ray

RGBColor	
Tracer::trace_ray(const Ray ray, const int depth) const {
	return (black);
}



