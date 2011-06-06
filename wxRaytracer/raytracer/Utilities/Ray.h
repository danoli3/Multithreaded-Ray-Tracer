#ifndef __RAY__
#define __RAY__

#include "Point3D.h"
#include "Vector3D.h"

class Ray {
	public:
	
		Point3D			o;  	// origin 
		Vector3D		d; 		// direction 
		
		Ray(void);			
		
		Ray(const Point3D& origin, const Vector3D& dir);	
		
		Ray(const Ray& ray); 		
		
		Ray& 						
		operator= (const Ray& rhs);
		 								
		~Ray(void);
};

#endif
