#ifndef __WORLD__
#define __WORLD__


// This file contains the declaration of the class World
// The World class does not have a copy constructor or an assignment operator, for the followign reasons:

// 1 	There's no need to copy construct or assign the World
// 2 	We wouldn't want to do this anyway, because the world can contain an arbitray amount of data
// 3 	These operations wouldn't work because the world is self-referencing:
//	 	the Tracer base class contains a pointer to the world. If we wrote a correct copy constructor for the 
// 	  	Tracer class, the World copy construtor would call itself recursively until we ran out of memory. 

#include <vector>
#include <list>

#include "ViewPlane.h"
#include "RGBColor.h"
#include "Tracer.h"
#include "GeometricObject.h"
#include "Sphere.h"
#include "SingleSphere.h"
#include "Ray.h"
#include "MultiThread.h"
#include "ReferenceCount.h"
#include "SmartPointer.h"
#include "Camera.h"
#include "Light.h"
#include "Ambient.h"
#include "RandomNumber.h"
 
using namespace std;

class RenderThread; 	//part of skeleton - wxRaytracer.h
class Image;

class World {	
	public:
	
		ViewPlane					vp;
		RGBColor					background_color;
		Tracer*						tracer_ptr;
		Light*   					ambient_ptr;
		Camera*						camera_ptr;		
		Sphere 						sphere;		// for Chapter 3 only
		vector<SmartPointer<GeometricObject> > objects;
		vector<SmartPointer<Light> > 				lights;
		
		RenderThread* 				paintArea; 	//connection to skeleton - wxRaytracer.h	
		RandomNumber*               random;

	public:
		World(void);
		~World();

		void 
		add_object(SmartPointer<GeometricObject> object_ptr);
								
		void 
		add_object(GeometricObject* object_ptr);
		
		void 
		add_light(Light* light_ptr); 
		
		void
		set_ambient_light(Light* light_ptr);			
		
		void
		set_camera(Camera* c_ptr);	 

		void 					
		build(void);

		void 												
		render_scene(void) const;
		
		void 												
		render_scene(const std::vector<Pixel>& pixels) const;
						
		RGBColor
		max_to_one(const RGBColor& c) const;
		
		RGBColor
		clamp_to_color(const RGBColor& c) const;
		
		void
		display_pixel(const int row, const int column, const RGBColor& pixel_color) const;
		
		void
		display_pixel(const list<RenderedPixel>& render) const;

		ShadeRec
		hit_objects(const Ray& ray);

		bool stop_rendering() const;

		RenderDisplay render_display() const;

		float 
		rand_float();

		float
		rand_float(int l, float h);

		unsigned long 
		rand_int();

		unsigned long
		rand_int(int l, int h);	

		void
		set_rand_seed(int seed);	
						
	private:
		
		void 
		delete_objects(void);
		
		void 
		delete_lights(void);
};


// ------------------------------------------------------------------ add_object

inline void 
World::add_object(SmartPointer<GeometricObject> object_ptr) {  
	objects.push_back(object_ptr);	

}

inline void 
World::add_object(GeometricObject* object_ptr) {  
	objects.push_back(object_ptr);	
}


// ------------------------------------------------------------------ add_light

inline void 
World::add_light(Light* light_ptr) {  
	lights.push_back(light_ptr);
}

// ------------------------------------------------------------------ set_ambient_light
inline void
World::set_ambient_light(Light* light_ptr) {
	if(ambient_ptr)
	{
		delete ambient_ptr;
		ambient_ptr = NULL;
	}
	ambient_ptr = light_ptr;
}

// ------------------------------------------------------------------ set_camera
inline void
World::set_camera(Camera* c_ptr) {
	if(camera_ptr)
	{
		delete camera_ptr;
		camera_ptr = NULL;
	}
	camera_ptr = c_ptr;
}

inline float World::rand_float()
{
	return random->rand_float();
}

inline float World::rand_float(int l, float h)
{
	return random->rand_float(l, h);
}

inline unsigned long World::rand_int()
{
	return random->rand_int();
}

inline unsigned long World::rand_int(int l, int h)
{
	return random->rand_int(l, h);
}

inline void World::set_rand_seed(int seed)
{
	random->set_rand_seed(seed);
}

#endif
