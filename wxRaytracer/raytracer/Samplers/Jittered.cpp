// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "Jittered.h"

// ---------------------------------------------------------------- default constructor
	
Jittered::Jittered(void)							
	: Sampler()
{}


// ---------------------------------------------------------------- constructor

Jittered::Jittered(const int num_samples)
	: Sampler(num_samples) {
	generate_samples();
}


// ---------------------------------------------------------------- constructor

Jittered::Jittered(const int num_samples, const int m)
	: Sampler(num_samples, m) {
	generate_samples();
}


// ---------------------------------------------------------------- copy constructor

Jittered::Jittered(const Jittered& js)			
	: Sampler(js) {
	generate_samples();
}

// ---------------------------------------------------------------- assignment operator

Jittered& 
Jittered::operator= (const Jittered& rhs) {
	if (this == &rhs)
		return (*this);
		
	Sampler::operator= (rhs);

	return (*this);
}

// ---------------------------------------------------------------- clone

Jittered*										
Jittered::clone(void) const {
	return (new Jittered(*this));
}

// ---------------------------------------------------------------- destructor			

Jittered::~Jittered(void) {}


// ---------------------------------------------------------------- generate_samples	

void
Jittered::generate_samples(void) {	
	int n = (int) sqrt((float)num_samples); 
	
	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < n; j++)		
			for (int k = 0; k < n; k++) {
				Point2D sp((k + rand_float()) / n, (j + rand_float()) / n);				
				samples.push_back(sp);
			}		
}

