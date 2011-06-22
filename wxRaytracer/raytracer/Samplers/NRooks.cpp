// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "NRooks.h"

// ---------------------------------------------------------------- default constructor
	
NRooks::NRooks(void)							
	: Sampler()
{}


// ---------------------------------------------------------------- constructor

NRooks::NRooks(const int num_samples)
	: 	Sampler(num_samples) {
	generate_samples();
}


// ---------------------------------------------------------------- constructor

NRooks::NRooks(const int num_samples, const int m)
	: 	Sampler(num_samples, m) {
	generate_samples();
}


// ---------------------------------------------------------------- copy constructor

NRooks::NRooks(const NRooks& nr)			
	: Sampler(nr) {
	generate_samples();
}

// ---------------------------------------------------------------- assignment operator

NRooks& 
NRooks::operator= (const NRooks& rhs) {
	if (this == &rhs)
		return (*this);
		
	Sampler::operator=(rhs);

	return (*this);
}

// ---------------------------------------------------------------- clone

NRooks*										
NRooks::clone(void) const {
	return (new NRooks(*this));
}

// ---------------------------------------------------------------- destructor			

NRooks::~NRooks(void) {}


// ---------------------------------------------------------------- generate_samples	

void
NRooks::generate_samples(void) {	
	for (int p = 0; p < num_sets; p++)          			
		for (int j = 0; j < num_samples; j++) {
			Point2D sp((j + rand_float()) / num_samples, (j + rand_float()) / num_samples);
			samples.push_back(sp);
		}		

	shuffle_x_coordinates();
	shuffle_y_coordinates();
}


	

