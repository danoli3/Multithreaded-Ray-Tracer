#ifndef __HAMMERSLEY__
#define __HAMMERSLEY__


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"

class Hammersley: public Sampler {
	public:
		
		Hammersley(void);							
		
		Hammersley(const int num);					

		Hammersley(const Hammersley& r);			

		Hammersley& 
		operator= (const Hammersley& rhs);			

		virtual Hammersley*							
		clone(void) const;

		virtual
		~Hammersley(void);
		
		double 
		phi(int j);
		
	private:
		
		virtual void
		generate_samples(void);
};

#endif

