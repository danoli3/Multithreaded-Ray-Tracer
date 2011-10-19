
// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include <algorithm>   // for random_shuffle in Sampler::setup_shuffled_indices

#include "Constants.h"
#include "Sampler.h"

// ------------------------------------------------------------------ default constructor
	
Sampler::Sampler(void)						
	: 	num_samples(1),
		num_sets(83),
		count(0),
		jump(0),
		random(new RandomNumber)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}


// ------------------------------------------------------------------ constructor

Sampler::Sampler(const int ns)
	: 	num_samples(ns),
		num_sets(83),
		count(0),
		jump(0),
		random(new RandomNumber)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}


// ------------------------------------------------------------------ constructor

Sampler::Sampler(const int ns, const int n_sets)
	: 	num_samples(ns),
		num_sets(n_sets),
		count(0),
		jump(0),
		random(new RandomNumber)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();	
}


// ------------------------------------------------------------------ copy constructor

Sampler::Sampler(const Sampler& s)				
	: 	num_samples(s.num_samples),
		num_sets(s.num_sets),
		samples(s.samples),
		shuffled_indices(s.shuffled_indices),
		disk_samples(s.disk_samples),
		hemisphere_samples(s.hemisphere_samples),
		sphere_samples(s.sphere_samples),
		count(s.count),
		jump(s.jump),
		random(new RandomNumber)
{ 
	/*if(s.random)
		random = s.random->clone(); 
	else random = NULL;*/


}


// ------------------------------------------------------------------ assignment operator

Sampler& 
Sampler::operator= (const Sampler& rhs)	{
	if (this == &rhs)
		return (*this);
	
	num_samples 		= rhs.num_samples;
	num_sets			= rhs.num_sets;
	samples				= rhs.samples;
	shuffled_indices	= rhs.shuffled_indices;
	disk_samples		= rhs.disk_samples;
	hemisphere_samples	= rhs.hemisphere_samples;
	sphere_samples		= rhs.sphere_samples;
	count				= rhs.count;
	jump				= rhs.jump;

	if (random) {
		delete random;
		random = NULL;
	}

	/*if (rhs.random)
		random = rhs.random->clone();*/	
	random = new RandomNumber;
	
	return (*this);
}

Sampler::~Sampler(void) 
{
	if (random) {
		delete random;
		random = NULL;
	}

}


// ------------------------------------------------------------------- set_num_sets

void
Sampler::set_num_sets(const int np) {
	num_sets = np;
}


// ------------------------------------------------------------------- get_num_samples

int
Sampler::get_num_samples(void) {
	return (num_samples);
}


// ------------------------------------------------------------------- shuffle_x_coordinates
// shuffle the x coordinates of the points within each set

void
Sampler::shuffle_x_coordinates(void) {
	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i <  num_samples - 1; i++) {
			int target = rand_int() % num_samples + p * num_samples;
			float temp = samples[i + p * num_samples + 1].x;
			samples[i + p * num_samples + 1].x = samples[target].x;
			samples[target].x = temp;
		}
}


// ------------------------------------------------------------------- shuffle_y_coordinates
// shuffle the y coordinates of the points within set

void
Sampler::shuffle_y_coordinates(void) {
	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i <  num_samples - 1; i++) {
			int target = rand_int() % num_samples + p * num_samples;
			float temp = samples[i + p * num_samples + 1].y;
			samples[i + p * num_samples + 1].y = samples[target].y;
			samples[target].y = temp;
		}	
}


// ------------------------------------------------------------------- setup_shuffled_indices
// sets up randomly shuffled indices for the samples array

void											
Sampler::setup_shuffled_indices(void) {
	shuffled_indices.reserve(num_samples * num_sets);
	vector<int> indices;
	
	for (int j = 0; j < num_samples; j++)
		indices.push_back(j);
	
	for (int p = 0; p < num_sets; p++) { 
		random_shuffle(indices.begin(), indices.end());	
		
		for (int j = 0; j < num_samples; j++)
			shuffled_indices.push_back(indices[j]);
	}	
}


// ------------------------------------------------------------------- map_samples_to_unit_disk

// Maps the 2D sample points in the square [-1,1] X [-1,1] to a unit disk, using Peter Shirley's
// concentric map function

void
Sampler::map_samples_to_unit_disk(void) {
	int size = samples.size();
	
	float r, phi;		// polar coordinates
	Point2D sp; 		// sample point on unit disk
	
	disk_samples.reserve(size);
		
	for (int j = 0; j < size; j++) {
		 // map sample point to [-1, 1] X [-1,1]
		 	
		sp.x = 2.0 * samples[j].x - 1.0;	
		sp.y = 2.0 * samples[j].y - 1.0;
			
		if (sp.x > -sp.y) {			// sectors 1 and 2
			if (sp.x > sp.y) {		// sector 1
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else {					// sector 2
				r = sp.y;
				phi = 2.0 - sp.x / sp.y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.x < sp.y) {		// sector 3
				r = -sp.x;
				phi = 4.0 + sp.y / sp.x;
			}
			else {					// sector 4
				r = -sp.y;
				if (sp.y != 0.0)	// avoid division by zero at origin
					phi = 6.0 - sp.x / sp.y;
				else
					phi  = 0.0;
			}
		}
		
		phi *= PI / 4.0;				
		disk_samples.push_back(Point2D((r * cos(phi)), r * sin(phi)));


	}
	
	samples.erase(samples.begin(), samples.end());
}


// ------------------------------------------------------------------- map_samples_to_hemisphere

// Maps the 2D sample points to 3D points on a unit hemisphere with a cosine power
// density distribution in the polar angle

void
Sampler::map_samples_to_hemisphere(const float exp) {
	int size = samples.size();
	hemisphere_samples.reserve(num_samples * num_sets);
		
	for (int j = 0; j < size; j++) {
		float cos_phi = cos(2.0 * PI * samples[j].x);
		float sin_phi = sin(2.0 * PI * samples[j].x);	
		float cos_theta = pow((1.0 - samples[j].y), 1.0 / (exp + 1.0));
		float sin_theta = sqrt (1.0 - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		hemisphere_samples.push_back(Point3D(pu, pv, pw)); 
	}
}


// ------------------------------------------------------------------- map_samples_to_sphere

// Maps the 2D sample points to 3D points on a unit sphere with a uniform density 
// distribution over the surface
// this is used for modelling a spherical light

void
Sampler::map_samples_to_sphere(void) {
	float r1, r2;
	float x, y, z;
	float r, phi;
		
	sphere_samples.reserve(num_samples * num_sets);   
		
	for (int j = 0; j < num_samples * num_sets; j++) {
		r1 	= samples[j].x;
    	r2 	= samples[j].y;
    	z 	= 1.0 - 2.0 * r1;
    	r 	= sqrt(1.0 - z * z);
    	phi = TWO_PI * r2;
    	x 	= r * cos(phi);
    	y 	= r * sin(phi);
		sphere_samples.push_back(Point3D(x, y, z)); 
	}
}


// ------------------------------------------------------------------- sample_unit_square
// the final version in Listing 5.13

Point2D
Sampler::sample_unit_square(void) {
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (rand_int() % num_sets) * num_samples;				// random index jump initialised to zero in constructor

	return (samples[jump + shuffled_indices[jump + count++ % num_samples]]);  
}
//-------------------------------------------------------------------- sample_unit_square MT
Point2D
Sampler::sample_unit_square(ShadeRec& sr) {	
	if(sr.sync == false) // if this is the first call to sample_unit_square this pixel
	{	sr.sync = true;
		if (*sr.count % num_samples == 0)					// start of a new pixel 									
		*sr.jump = (rand_int() % num_sets) * num_samples;	// so set jump to a new random number
		return (samples[*sr.jump + shuffled_indices[*sr.jump + (*sr.count)++ % num_samples]]); }
	else				// not the first call to sample_unit_square this pixel
		return (samples[*sr.jump + shuffled_indices[*sr.jump + *sr.count % num_samples]]);

}

// the final version in Listing 5.13

Point2D
Sampler::sample_unit_square(int &count, int &jumper) {
	if (count % num_samples == 0)					// start of a new pixel 									
		jumper = (rand_int() % num_sets) * num_samples;	// so set jump to a new random number
	return (samples[jumper + shuffled_indices[jumper + (count)++ % num_samples]]); 
}


/*

// ------------------------------------------------------------------- sample_unit_square
// the first revised version in Listing in Listing 5.8

Point2D
Sampler::sample_unit_square(void) {
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (rand_int() % num_sets) * num_samples;				// random index jump initialised to zero in constructor
	
	return (samples[jump + count++ % num_samples]);	
}

*/


/*

// ------------------------------------------------------------------- sample_unit_square
// the original version in Listing 5.7

Point2D
Sampler::sample_unit_square(void) {
	return (samples[count++ % (num_samples * num_sets)]);
}

*/


// ------------------------------------------------------------------- sample_unit_disk

Point2D
Sampler::sample_unit_disk(void) {
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (rand_int() % num_sets) * num_samples;
	
	return (disk_samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

// ------------------------------------------------------------------- sample_unit_disk MT

Point2D
Sampler::sample_unit_disk(ShadeRec& sr) {
	if(sr.sync == false) // if this is the first call to sample_unit_disk this pixel
	{	sr.sync = true;
		if (*sr.count % num_samples == 0)					// start of a new pixel 									
		*sr.jump = (rand_int() % num_sets) * num_samples;	// so set jump to a new random number
		return (disk_samples[*sr.jump + shuffled_indices[*sr.jump + (*sr.count)++ % num_samples]]); }
	else				// not the first call to sample_unit_disk this pixel
		return (disk_samples[*sr.jump + shuffled_indices[*sr.jump + *sr.count % num_samples]]);
}


// ------------------------------------------------------------------- sample_hemisphere

Point3D
Sampler::sample_hemisphere(void) {
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (rand_int() % num_sets) * num_samples;
		
	return (hemisphere_samples[jump + shuffled_indices[jump + count++ % num_samples]]);		
}


// ------------------------------------------------------------------- sample_hemisphere MT
Point3D
Sampler::sample_hemisphere(ShadeRec& sr)
{	if(sr.sync == false) // if this is the first call to sample_hemisphere this pixel
	{	sr.sync = true;
		if (*sr.count % num_samples == 0)					// start of a new pixel 									
		*sr.jump = (rand_int() % num_sets) * num_samples;	// so set jump to a new random number
		return (hemisphere_samples[*sr.jump + shuffled_indices[*sr.jump + (*sr.count)++ % num_samples]]); }
	else				// not the first call to sample_hemisphere this pixel
		return (hemisphere_samples[*sr.jump + shuffled_indices[*sr.jump + *sr.count % num_samples]]);

}


// ------------------------------------------------------------------- sample_sphere

Point3D
Sampler::sample_sphere(void) {
	if (count % num_samples == 0)  									// start of a new pixel
		jump = (rand_int() % num_sets) * num_samples;	
	return (sphere_samples[jump + shuffled_indices[jump + count++ % num_samples]]);		
}

// ------------------------------------------------------------------- sample_sphere MT

Point3D
Sampler::sample_sphere(ShadeRec& sr) {	
	if(sr.sync == false) // if this is the first call to sample_sphere this pixel
	{	sr.sync = true;
		if (*sr.count % num_samples == 0)					// start of a new pixel 									
		*sr.jump = (rand_int() % num_sets) * num_samples;	// so set jump to a new random number
		return (sphere_samples[*sr.jump + shuffled_indices[*sr.jump + (*sr.count)++ % num_samples]]); }
	else				// not the first call to sample_sphere this pixel
		return (sphere_samples[*sr.jump + shuffled_indices[*sr.jump + *sr.count % num_samples]]);
}



// ------------------------------------------------------------------- sample_one_set
// This is a specialised function called in LatticeNoise::init_vector_table
// It doesn't shuffle the indices

Point2D
Sampler::sample_one_set(void) {
	return(samples[count++ % num_samples]);  
}
// ------------------------------------------------------------------- sample_one_set MT
Point2D
Sampler::sample_one_set(ShadeRec& sr) {
	return(samples[(*sr.count)++ % num_samples]);  
}




