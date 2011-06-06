#include "Material.h"

// ---------------------------------------------------------------- default constructor

Material::Material(void) {}


// ---------------------------------------------------------------- copy constructor

Material::Material(const Material& m) {}



// ---------------------------------------------------------------- assignment operator

Material& 
Material::operator= (const Material& rhs) {
	if (this == &rhs)
		return (*this);

	return (*this);
}


// ---------------------------------------------------------------- destructor

Material::~Material(void)
{}


// ---------------------------------------------------------------- shade

RGBColor
Material::shade(ShadeRec& sr) {
	return (black);
}



