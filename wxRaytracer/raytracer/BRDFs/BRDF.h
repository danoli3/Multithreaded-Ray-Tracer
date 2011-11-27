#ifndef __BRDF__
#define __BRDF__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

// BRDF is the base class for the BRDFs
// we need a sampler here, because even a Matte material needs to call sample_f with a 
// cosine distribution for path shading. 

//#include <math.h>

#include "RGBColor.h"
#include "Vector3D.h"
#include "ShadeRec.h"
#include "Sampler.h"
#include "ReferenceCount.h"
#include "SmartPointer.h"

class BRDF : public ReferenceCount {
	public:
	
		BRDF(void);						
		
		BRDF(const BRDF& brdf);		
		
		virtual BRDF*
		clone(void)const = 0;
		
		BRDF&							
		operator= (const BRDF& rhs);
		
		virtual
		~BRDF(void);
				
		void							
		set_sampler(Sampler* sPtr);
		
		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
		
		virtual RGBColor
		sample_f(ShadeRec& sr, const Vector3D& wo, Vector3D& wi);
		
		virtual RGBColor
		sample_f(ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf);
		
		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;
		
			
	protected:
	
		SmartPointer<Sampler> sampler_ptr;		// for indirect illumination
};

#endif

