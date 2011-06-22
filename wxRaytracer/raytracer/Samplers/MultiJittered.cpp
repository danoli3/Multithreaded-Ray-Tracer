// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "MultiJittered.h"

// ---------------------------------------------------------------- default constructor
	
MultiJittered::MultiJittered(void)							
	: Sampler()
{}


// ---------------------------------------------------------------- constructor

MultiJittered::MultiJittered(const int num_samples)
	: 	Sampler(num_samples) {
//	count = 0;
//	jump = 0;
	generate_samples();
}


// ---------------------------------------------------------------- constructor

MultiJittered::MultiJittered(const int num_samples, const int m)
	: 	Sampler(num_samples, m) {
	generate_samples();
}


// ---------------------------------------------------------------- copy constructor

MultiJittered::MultiJittered(const MultiJittered& mjs)			
	: Sampler(mjs)
{
	generate_samples();
}

// ---------------------------------------------------------------- assignment operator

MultiJittered& 
MultiJittered::operator= (const MultiJittered& rhs)	{
	if (this == &rhs)
		return (*this);
		
	Sampler::operator=(rhs);

	return (*this);
}

// ---------------------------------------------------------------- clone

MultiJittered*										
MultiJittered::clone(void) const {
	return (new MultiJittered(*this));
}

// ---------------------------------------------------------------- destructor			

MultiJittered::~MultiJittered(void) {}


// ---------------------------------------------------------------- shuffle_x_coordinates

/*

void
MultiJittered::shuffle_x_coordinates(void) {
	for (int p = 0; p < num_sets; p++) {
			for (int i = 0; i < n; i++)		
				for (int j = 0; j < n; j++) {
					int k = rand_int(j, n - 1);
					float t = samples[i * n + j + p * num_samples].x;
					samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
					samples[i * n + k + p * num_samples].x = t;
				}
		}
}

*/


// ---------------------------------------------------------------- generate_samples

// This is based on code in Chui et al. (1994), cited in the references	
// The overloaded functions rand_int and rand_float (called from rand_int), which take arguments,
// are defined in Maths.h
// They should be defined here, as this is the only place they are usedm but I couldn't get them to compile

void
MultiJittered::generate_samples(void) {		
	// num_samples needs to be a perfect square
			
	int n = (int)sqrt((float)num_samples);
	float subcell_width = 1.0 / ((float) num_samples);
	
	// fill the samples array with dummy points to allow us to use the [ ] notation when we set the 
	// initial patterns
	
	Point2D fill_point;
	for (int j = 0; j < num_samples * num_sets; j++)
		samples.push_back(fill_point);
		
	// distribute points in the initial patterns
	
	for (int p = 0; p < num_sets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				samples[i * n + j + p * num_samples].x = (i * n + j) * subcell_width + rand_float(0, subcell_width);
				samples[i * n + j + p * num_samples].y = (j * n + i) * subcell_width + rand_float(0, subcell_width);
			}
	
	// shuffle x coordinates
	
	for (int p = 0; p < num_sets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = rand_int(j, n - 1);
				float t = samples[i * n + j + p * num_samples].x;
				samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
				samples[i * n + k + p * num_samples].x = t;
			}

	// shuffle y coordinates
	
	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = rand_int(j, n - 1);
				float t = samples[j * n + i + p * num_samples].y;
				samples[j * n + i + p * num_samples].y = samples[k * n + i + p * num_samples].y;
				samples[k * n + i + p * num_samples].y = t;
		}
}


