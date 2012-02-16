#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__
	
#include "Point3D.h"
#include "Ray.h"
#include "ShadeRec.h"

#include "ReferenceCount.h"
#include "SmartPointer.h"

class Material;


//----------------------------------------------------------------------------------------------------- Class GeometricObject

class GeometricObject : public ReferenceCount {	
	
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
		set_material(Material* mPtr); 			// It therefore shouldn't be inlined
		
		virtual void 
		set_material(SmartPointer<Material> mPtr); 			// It therefore shouldn't be inlined
		

		virtual void 										// required for compound objects
		add_object(GeometricObject* object_ptr);

		virtual void 										// required for compound objects
		add_object(SmartPointer<GeometricObject> object_ptr);

	
	protected:
		SmartPointer<Material>   material_ptr;  
		
		GeometricObject&						// assignment operator
		operator= (const GeometricObject& rhs);
};


// ------------------------------------------------------------------------- get_material

inline Material* 
GeometricObject::get_material(void) const {
	return (material_ptr.get_pointer());
}

#endif