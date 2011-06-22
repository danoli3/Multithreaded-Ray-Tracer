#ifndef __JITTERED__
#define __JITTERED__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"

class Jittered: public Sampler {
	public:
		
		Jittered(void);								
		
		Jittered(const int num_samples);					
		
		Jittered(const int num_samples, const int m);	

		Jittered(const Jittered& u);					

		Jittered& 
		operator= (const Jittered& rhs);				

		virtual Jittered*								
		clone(void) const;			

		virtual
		~Jittered(void);
		
	private:
		
		virtual void									
		generate_samples(void);
};

#endif

