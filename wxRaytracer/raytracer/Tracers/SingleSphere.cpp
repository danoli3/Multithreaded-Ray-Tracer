#include "SingleSphere.h"
#include "World.h"
#include "ShadeRec.h"

// -------------------------------------------------------------------- default constructor

SingleSphere::SingleSphere(void)
	: Tracer()
{}


// -------------------------------------------------------------------- constructor
		
SingleSphere::SingleSphere(World* _worldPtr)
	: Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

SingleSphere::~SingleSphere(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor	
SingleSphere::trace_ray(const Ray& ray) const {
	ShadeRec	sr(*world_ptr); 	// not used
	double    	t;  				// not used
	
	if (world_ptr->sphere.hit(ray, t, sr))		
		return (red);  
	else
		return (black);   
}


