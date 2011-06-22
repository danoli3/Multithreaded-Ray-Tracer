#ifndef __PURE_RANDOM__
#define __PURE_RANDOM__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"

class PureRandom: public Sampler {
	public:
		
		PureRandom(void);							
		
		PureRandom(const int num);					

		PureRandom(const PureRandom& r);				

		PureRandom& 
		operator= (const PureRandom& rhs);			

		virtual PureRandom*							
		clone(void) const;			

		virtual
		~PureRandom(void);
		
	private:
		
		virtual void
		generate_samples(void);
};

#endif

