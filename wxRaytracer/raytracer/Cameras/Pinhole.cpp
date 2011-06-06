// This file contains the definition of the Pinhole class

#include "Constants.h" 
#include "Point3D.h"
#include "Vector3D.h"
#include "Pinhole.h"
#include <math.h>
#include <list>

// ----------------------------------------------------------------------------- default constructor

Pinhole::Pinhole(void)		
	:	Camera(),
		d(500),
		zoom(1.0)
{}


// ----------------------------------------------------------------------------- copy constructor

Pinhole::Pinhole(const Pinhole& c)   		
	: 	Camera(c),
		d(c.d),
		zoom(c.zoom)
{}


// ----------------------------------------------------------------------------- clone

Camera* 
Pinhole::clone(void) const {
	return (new Pinhole(*this));
}



// ----------------------------------------------------------------------------- assignment operator

Pinhole& 
Pinhole::operator= (const Pinhole& rhs) { 	
	if (this == &rhs)
		return (*this);
		
	Camera::operator= (rhs);

	d 		= rhs.d;
	zoom	= rhs.zoom;

	return (*this);
}


// ----------------------------------------------------------------------------- destructor

Pinhole::~Pinhole(void) {}	


// ----------------------------------------------------------------------------- get_direction

Vector3D
Pinhole::get_direction(const Point2D& p) const {
	Vector3D dir = p.x * u + p.y * v - d * w;
	dir.normalize();
	
	return(dir);
}



// ----------------------------------------------------------------------------- render_scene

void 												
Pinhole::render_scene(const World& w) {
	RGBColor	L;
	ViewPlane	vp(w.vp);	 								
	Ray			ray;
	int 		depth = 0;  
	Point2D 	pp;		// sample point on a pixel
	int n = (int)sqrt((float)vp.num_samples);
		
	vp.s /= zoom;
	ray.o = eye;
		
	for (int r = 0; r < vp.vres; r++)			// up
		for (int c = 0; c < vp.hres; c++) {		// across 					
			L = black; 
			
			for (int p = 0; p < n; p++)			// up pixel
				for (int q = 0; q < n; q++) {	// across pixel
					pp.x = vp.s * (c - 0.5 * vp.hres + (q + 0.5) / n); 
					pp.y = vp.s * (r - 0.5 * vp.vres + (p + 0.5) / n);
					ray.d = get_direction(pp);
					L += w.tracer_ptr->trace_ray(ray, depth);
				}	
											
			L /= vp.num_samples;
			L *= exposure_time;
			w.display_pixel(r, c, L);
		} 
}

void 																		
Pinhole::render_scene(const World& w, const PixelPoints& grid)
{
	RGBColor	L;
	ViewPlane	vp(w.vp);	 								
	Ray			ray;
	int 		depth = 0;  // recusrion depth	
	Point2D 	pp;			// sample point on a pixel
	int n = (int)sqrt((float)vp.num_samples);
	
	vp.s /= zoom;
	ray.o = eye;

	list<RenderedPixel> render;   // for send every row
	RenderedPixel pixel;		  // "

	for (int r = grid.origin.y; r < grid.end.y; r++)
	{// up
		for (int c = grid.origin.x; c < grid.end.x; c++) {		// across 					
			L = black; 			
			for (int p = 0; p < n; p++)			// up pixel
				for (int q = 0; q < n; q++) {	// across pixel
					pp.x = vp.s * (c - 0.5 * vp.hres + (q + 0.5) / n); 
					pp.y = vp.s * (r - 0.5 * vp.vres + (p + 0.5) / n);
					ray.d = get_direction(pp);
					L += w.tracer_ptr->trace_ray(ray, depth);
				}	
											
			L /= vp.num_samples;
			L *= exposure_time;

			pixel.color = L;			// for send every row
			pixel.xy = Point2D(c,r);	// "
			render.push_back(pixel);    // "
			//w.display_pixel(r, c, L);   // for send to screen every pixel
		} 
		w.display_pixel(render);   // for send every row
		render.clear();		       // "
	}
}


