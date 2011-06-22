#ifndef __N_ROOKS__
#define __N_ROOKS__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"

class NRooks: public Sampler {
	public:
		
		NRooks(void);									
		
		NRooks(const int num_samples);					
		
		NRooks(const int num_samples, const int m);	

		NRooks(const NRooks& nr);					

		NRooks& 
		operator= (const NRooks& rhs);				

		virtual NRooks*								
		clone(void) const;			

		virtual
		~NRooks(void);
		
	private:
		
		virtual void									
		generate_samples(void);						
};

#endif

