// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Constants.h"
#include "BRDF.h"

// ---------------------------------------------------------- default constructor

BRDF::BRDF(void) 
	: sampler_ptr(NULL)
{}


// ---------------------------------------------------------- copy constructor

BRDF::BRDF (const BRDF& brdf) {
	if(brdf.sampler_ptr)
		sampler_ptr	= brdf.sampler_ptr->clone(); 
	else  sampler_ptr = NULL;
}	



// --------------------------------------------------------------- assignment operator

BRDF&														
BRDF::operator= (const BRDF& rhs) {
	if (this == &rhs)
		return (*this);
		
	/*if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}*/

	//if (rhs.sampler_ptr)
		sampler_ptr	= rhs.sampler_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------- destructor

BRDF::~BRDF(void) {
	/*if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}*/
}  



// ---------------------------------------------------------- set_sampler

void
BRDF::set_sampler(Sampler* sPtr) {
	sampler_ptr = sPtr;
	sampler_ptr->map_samples_to_hemisphere(1);  // for perfect diffuse
}


// ------------------------------------------------------------------------ f

RGBColor
BRDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
	return (black);
}


// ------------------------------------------------------------------------ sample_f

RGBColor
BRDF::sample_f(ShadeRec& sr, const Vector3D& wo, Vector3D& wi) {
	return (black);
}


// ------------------------------------------------------------------------ sample_f

RGBColor
BRDF::sample_f(ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) {
	return (black);
}


// ------------------------------------------------------------------------ rho	
	
RGBColor
BRDF::rho(const ShadeRec& sr, const Vector3D& wo) const {
	return (black);
}

