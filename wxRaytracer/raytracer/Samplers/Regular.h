#ifndef __REGULAR__
#define __REGULAR__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"

class Regular: public Sampler {
	public:
		
		Regular(void);							
		
		Regular(const int num);					

		Regular(const Regular& u);				

		Regular& 
		operator= (const Regular& rhs);			

		virtual Regular*						
		clone(void) const;			

		virtual
		~Regular(void);
		
		virtual void
		generate_samples(void);
};

#endif

