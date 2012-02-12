#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "BRDF.h"

class Lambertian: public BRDF {
	public:
	
		Lambertian(void);
		
		Lambertian(const Lambertian& lamb);
		
		Lambertian& 
		operator= (const Lambertian& rhs);
		
		virtual
		~Lambertian(void);
		
		virtual Lambertian*
		clone(void) const;
		
		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
		
		virtual RGBColor												
		sample_f(ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf);
		
		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;
			
		void
		set_ka(const float ka);	
				
		void
		set_kd(const float kd);
		
		void
		set_cd(const RGBColor& c);
		
		void													
		set_cd(const float r, const float g, const float b);
		
		void													
		set_cd(const float c);

	    float													
		get_kd() const;

		RGBColor													
		get_cd() const;
					
	private:
	
		float		kd;
		RGBColor 	cd;
};




// -------------------------------------------------------------- set_ka

inline void
Lambertian::set_ka(const float k) {
	kd = k;
}



// -------------------------------------------------------------- set_kd

inline void
Lambertian::set_kd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- set_cd

inline void
Lambertian::set_cd(const RGBColor& c) {
	cd = c;
}


// ---------------------------------------------------------------- set_cd

inline void													
Lambertian::set_cd(const float r, const float g, const float b) {
	cd.r = r; cd.g = g; cd.b = b;
}


// ---------------------------------------------------------------- set_cd

inline void													
Lambertian::set_cd(const float c) {
	cd.r = c; cd.g = c; cd.b = c;
}

#endif
