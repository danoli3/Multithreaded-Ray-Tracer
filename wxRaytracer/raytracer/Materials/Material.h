#ifndef __MATERIAL__
#define __MATERIAL__

#include "World.h"			// required for the shade function in all derived classes
#include "RGBColor.h"
#include "ShadeRec.h"

class Material {	
	public:
	
		Material(void);						
		
		Material(const Material& material); 
		
		virtual Material*								
		clone(void) const = 0;	
				
		virtual 								
		~Material(void);
				
		virtual RGBColor
		shade(ShadeRec& sr);	
		
	protected:
	
		Material& 								
		operator= (const Material& rhs);						
};

#endif
