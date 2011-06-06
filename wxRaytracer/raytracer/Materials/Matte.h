#ifndef __MATTE__
#define __MATTE__

#include "Material.h"
#include "Lambertian.h"

//----------------------------------------------------------------------------- class Matte

class Matte: public Material {	
	public:
			
		Matte(void);											

		Matte(const Matte& m);
		
		virtual Material*										
		clone(void) const;									

		Matte& 
		operator= (const Matte& rhs);							

		~Matte(void);											
		
		void 													
		set_ka(const float k);
		
		void 													
		set_kd(const float k);
		
		void													
		set_cd(const RGBColor c);
		
		void													
		set_cd(const float r, const float g, const float b);
		
		void																						
		set_cd(const float c);
				
		virtual RGBColor										
		shade(ShadeRec& sr);
		
	private:
		
		Lambertian*		ambient_brdf;
		Lambertian*		diffuse_brdf;
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection 
// is diffuse reflection

inline void								
Matte::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void								
Matte::set_kd (const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void												
Matte::set_cd(const RGBColor c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd

inline void													
Matte::set_cd(const float r, const float g, const float b) {
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
}

// ---------------------------------------------------------------- set_cd

inline void													
Matte::set_cd(const float c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}

#endif