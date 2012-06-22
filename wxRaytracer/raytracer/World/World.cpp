
// this file contains the definition of the World class

#include "wxraytracer.h"

#include "World.h"
#include "Constants.h"

// geometric objects

#include "Plane.h"
#include "Sphere.h"

// tracers

#include "SingleSphere.h"
#include "MultipleObjects.h"
#include "RayCast.h"

// cameras

#include "Pinhole.h"

// lights

#include "Directional.h"

// materials

#include "Material.h"
#include "Matte.h"

// utilities

#include "Vector3D.h"
#include "Point3D.h"
#include "Normal.h"
#include "ShadeRec.h"
#include "Maths.h"

// build functions

#include "BuildShadedObjects.cpp"


// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
	:  	background_color(black),
		tracer_ptr(NULL),
		ambient_ptr(new Ambient),
		camera_ptr(NULL),
		random(new RandomNumber())

{}

//------------------------------------------------------------------ destructor
World::~World(void) {	
	
	if(tracer_ptr) {
		delete tracer_ptr;
		tracer_ptr = NULL;
	}
	
		
	if (ambient_ptr) {
		delete ambient_ptr;
		ambient_ptr = NULL;
	}
			
		
	if (camera_ptr) {
		delete camera_ptr;
		camera_ptr = NULL;
	}

	if (random) {
		delete random;
		random = NULL;
	}
	
	delete_objects();	
	delete_lights();	
}

//------------------------------------------------------------------ render_scene
void 												
World::render_scene(void) const {

	RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 100.0;				// hardwired in

	ray.d = Vector3D(0, 0, -1);
	
	for (int r = 0; r < vres; r++)			// up
		for (int c = 0; c <= hres; c++) {	// across 					
			ray.o = Point3D(s * (c - hres / 2.0 + 0.5), s * (r - vres / 2.0 + 0.5), zw);
			pixel_color = tracer_ptr->trace_ray(ray);
			display_pixel(r, c, pixel_color);
		}	
}  

// This uses orthographic viewing along the zw axis (Multithreaded)

void 												
World::render_scene(const std::vector<Pixel>& pixels) const {

	RGBColor	pixel_color;	 	
	Ray			ray;					
	int 		hres 	= vp.hres;
	int 		vres 	= vp.vres;
	float		s		= vp.s;
	float		zw		= 100.0;				// hardwired in
	
	list<RenderedPixel> render;   // for send every row
	RenderedPixel pixel;		  // "
	int count = 0;
	int jump  = 0;
	int depth = 0;

	ray.d = Vector3D(0, 0, -1);
	
	for(unsigned int i = 0; i< pixels.size(); i++)
	{			
			Pixel screen_pixel = pixels[i];		
			ray.o = Point3D(s * (screen_pixel.x - hres / 2.0 + 0.5), s * (screen_pixel.y - vres / 2.0 + 0.5), zw);
			pixel_color = tracer_ptr->trace_ray(ray, depth, count, jump);
			pixel.color = pixel_color;			// for send every row
			pixel.xy = Point2D(screen_pixel.x,screen_pixel.y);	// "
			render.push_back(pixel);    // "

				if(stop_rendering())       // if the program is asked to close, we need end this now
				{	display_pixel(render);  
					render.clear();	
					return; 	}	

				if(render_display() == EVERY_PIXEL)
				{	display_pixel(render);   // send to the screen buffer every pixel rendered
					render.clear();	
				}
				else if(render_display() == EVERY_ROW)
				{	
					if(i % (pixels.size()/10) == 0)
					{
						display_pixel(render);   // send to the screen buffer every pixel rendered
						render.clear();	
					}
				}
	}	
	if(render_display() == EVERY_JOB || render_display() == EVERY_ROW)
	{	display_pixel(render);   // send to the screen buffer every row of pixels rendered
		render.clear();	
	}		

	
}  

// ------------------------------------------------------------------ clamp

RGBColor
World::max_to_one(const RGBColor& c) const  {
	float max_value = max(c.r, max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}

// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one

RGBColor
World::clamp_to_color(const RGBColor& raw_color) const {
	RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}		
	return (c);
}

// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function


void
World::display_pixel(const int row, const int column, const RGBColor& raw_color) const {
	RGBColor mapped_color;

	if (vp.show_out_of_gamut)
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);
	
	if (vp.gamma != 1.0)
		mapped_color = mapped_color.powc(vp.inv_gamma);
	
   //have to start from max y coordinate to convert to screen coordinates
   int x = column;
   int y = vp.vres - row - 1;

   paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
                             (int)(mapped_color.g * 255),
                             (int)(mapped_color.b * 255));
}


void
World::display_pixel(const list<RenderedPixel>& render) const {
	
	list<RenderedPixel>::const_iterator it;	
	//list<RenderedInt> rendered;
	//vector<RenderedInt> rendered;
	vector<RenderPixel*> rendered;

	rendered.reserve(render.size());
	for ( it=render.begin() ; it != render.end(); it++)
	{   
		RGBColor mapped_color;

		if (vp.show_out_of_gamut)
			mapped_color = clamp_to_color(it->color);
		else
			mapped_color = max_to_one(it->color);
	
		if (vp.gamma != 1.0)
			mapped_color = mapped_color.powc(vp.inv_gamma);

	  
	
	   //have to start from max y coordinate to convert to screen coordinates
	   //int x = it->xy.x;
	   int y = vp.vres - it->xy.y - 1;
	   /*RenderedInt rend;
	   rend.x = it->xy.x;
	   rend.y = y;
	   rend.r = (int)(mapped_color.r * 255);
       rend.g = (int)(mapped_color.g * 255);
       rend.b = (int)(mapped_color.b * 255);
	   rendered.push_back(rend);*/

	   rendered.push_back(new RenderPixel);
	   rendered.back()->x = it->xy.x;
	   rendered.back()->y = y;
	   rendered.back()->red = (int)(mapped_color.r * 255);
       rendered.back()->green = (int)(mapped_color.g * 255);
       rendered.back()->blue = (int)(mapped_color.b * 255);	 
	}
	
   paintArea->setPixel(rendered);
   rendered.clear();
}

// ----------------------------------------------------------------------------- hit_objects

ShadeRec									
World::hit_objects(const Ray& ray) {

	ShadeRec	sr(*this); 
	double		t;
	Normal normal;
	Point3D local_hit_point;
	double		tmin 			= HUGE_VALUE;
	int 		num_objects 	= objects.size();
	
	for (int j = 0; j < num_objects; j++)
		if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object	= true;
			tmin 				= t;
			if(objects[j]->get_material())
				sr.material_ptr     = objects[j]->get_material();
			sr.hit_point 		= ray.o + t * ray.d;
			normal 				= sr.normal;
			local_hit_point	 	= sr.local_hit_point;
		}  
	if(sr.hit_an_object) {
		sr.t = tmin;
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}		
	return(sr);   
}

//------------------------------------------------------------------ delete_objects
// Deletes the objects in the objects array, and erases the array.
// The objects array still exists, because it's an automatic variable, but it's empty 
void
World::delete_objects(void) {
	int num_objects = objects.size();
	
	for (int j = 0; j < num_objects; j++) {
		// This is the code that was used before the SmartPointer.
		//delete objects[j];
		//objects[j] = NULL;
		// Now we can call an Explicit Delete on the Stored Object (However it is not nessessary).
		//objects[j].delete_now();
		
	}		
	objects.erase (objects.begin(), objects.end());

}

//------------------------------------------------------------------ delete_lights
void
World::delete_lights(void) {
	int num_lights = lights.size();
	
	// Handled by SmartPointer :)
	/*for (int j = 0; j < num_lights; j++) {
		delete lights[j];
		lights[j] = NULL;
	}	*/
	
	lights.erase (lights.begin(), lights.end());
}

bool
World::stop_rendering() const
{
	if(paintArea != NULL)
		return paintArea->Stop();
	else
		return true;
}

RenderDisplay
World::render_display() const
{
	if(paintArea != NULL)
		return paintArea->Display();
	else
		return EVERY_ROW;
}

