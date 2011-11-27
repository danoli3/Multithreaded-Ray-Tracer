// this file contains the definition of the class GeometricObject 

#include "Constants.h"
#include "Material.h"
#include "GeometricObject.h"


// ---------------------------------------------------------------------- default constructor


GeometricObject::GeometricObject(void)
	: material_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject (const GeometricObject& object) {
	if(object.material_ptr)
		material_ptr = object.material_ptr->clone(); 
	//else  material_ptr = NULL;
}	


// ---------------------------------------------------------------------- assignment operator

GeometricObject&														
GeometricObject::operator= (const GeometricObject& rhs) {
	if (this == &rhs)
		return (*this);
			
	// Material Deletion handled by the SmartPointer
	if (rhs.material_ptr)
		material_ptr = rhs.material_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject (void) {	
	// Material Deletion handled by the SmartPointer
}


// ---------------------------------------------------------------- set_material

void 
GeometricObject::set_material(Material* mPtr) {
	material_ptr = mPtr;
}

void 
GeometricObject::set_material(SmartPointer<Material> mPtr) {
	material_ptr = mPtr;
}

// ---------------------------------------------------------------------- add_object
// required for Compound objects 

void 												
GeometricObject::add_object(GeometricObject* object_ptr) {}

void 												
GeometricObject::add_object(SmartPointer<GeometricObject> object_ptr) {}

								