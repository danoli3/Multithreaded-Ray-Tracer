#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__

class Material;
	
#include "Point3D.h"
#include "Ray.h"
#include "ShadeRec.h"


//----------------------------------------------------------------------------------------------------- Class GeometricObject

class GeometricObject {	
	
	public:	

		GeometricObject(void);									// default constructor
		
		GeometricObject(const GeometricObject& object);			// copy constructor
	
		virtual GeometricObject*								// virtual copy constructor
		clone(void) const = 0;

		virtual 												// destructor
		~GeometricObject (void);	
			
		virtual bool 												 
		hit(const Ray& ray, double& t, ShadeRec& s) const = 0;
				
		Material*						
		get_material(void) const;

		virtual void 							// needs to virtual so that it can be overriden in Compound
		set_material(Material* mPtr); 			

	
	protected:
	
		mutable Material*   material_ptr;   	// mutable allows Compound::hit, Instance::hit and Grid::hit to assign to material_ptr. hit functions are const
	
		GeometricObject&						// assignment operator
		operator= (const GeometricObject& rhs);
};


// ------------------------------------------------------------------------- get_material

inline Material* 
GeometricObject::get_material(void) const {
	return (material_ptr);
}

#endif