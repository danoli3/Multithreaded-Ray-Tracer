// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "Regular.h"

// ---------------------------------------------------------------- default constructor
	
Regular::Regular(void)							
	: Sampler()
{}


// ---------------------------------------------------------------- constructor

Regular::Regular(const int num)
	: 	Sampler(num) 
{
	generate_samples();
}


// ---------------------------------------------------------------- copy constructor

Regular::Regular(const Regular& u)			
	: Sampler(u)
{
	generate_samples();
}

// ---------------------------------------------------------------- assignment operator

Regular& 
Regular::operator= (const Regular& rhs)	{
	if (this == &rhs)
		return (*this);
		
	Sampler::operator= (rhs);

	return (*this);
}

// ---------------------------------------------------------------- clone

Regular*										
Regular::clone(void) const {
	return (new Regular(*this));
}

// ---------------------------------------------------------------- destructor			

Regular::~Regular(void) {}


// ---------------------------------------------------------------- generate_samples	

void
Regular::generate_samples(void) {
	int n = (int) sqrt((float)num_samples);

	for (int j = 0; j < num_sets; j++)
		for (int p = 0; p < n; p++)		
			for (int q = 0; q < n; q++)
				samples.push_back(Point2D((q + 0.5) / n, (p + 0.5) / n));
}
