#ifndef __RANDOMNUMBER__
#define __RANDOMNUMBER__

//  Copyright (C) 2011 Daniel Rosser
//	This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program. See /LICENSES/gpl-3.0.txt
//    If not, see <http://www.gnu.org/licenses/>.
//
//    Daniel Rosser <danoli3@gmail.com> 

#include "Constants.h"
#include "MTRand.h"


class RandomNumber {
public:	

	RandomNumber()
		:mersenne_twister(true)		
	{ 		
		mtrand = new MTRand_int32;
	}

	RandomNumber(bool is_mersenne_twister)
		:mersenne_twister(mersenne_twister)
	{		
		mtrand = new MTRand_int32;
	}

	~RandomNumber(void)
	{
		if(mtrand)
		{
			delete mtrand;
			mtrand = NULL;
		}
	}

	RandomNumber* 
	clone (void) const {
		return (new RandomNumber(*this));
	}
	

	unsigned long		
	rand_int(void);

	unsigned long		
	rand_int(int l, int h);

	float   
	rand_float(void);

	float   
	rand_float(float);

	float  
	rand_float(int l, float h);

	float   
	rand_float_between(float mn, float mx);

	void    
	set_rand_seed(const int seed);	

	float
	getrand();

private: 
	bool mersenne_twister;
	MTRand_int32 *mtrand;

};

// ---------------------------------------------------- rand_int
// a wrapper for rand()

inline unsigned long
RandomNumber::rand_int(void) {
	if(mersenne_twister)
		return mtrand->random();
	else
		return(rand());
	
}


// ---------------------------------------------------- rand_float

inline float
RandomNumber::rand_float(void) {
	if(mersenne_twister)
		return ((float)rand_int() * INV_MT_RAND_MAX);
	else
		return((float)rand_int() * INV_RAND_MAX);
	
}


// ---------------------------------------------------- set_rand_seed

inline void
RandomNumber::set_rand_seed(const int seed) {
	if(mersenne_twister)
		mtrand->seed(seed);
	else
		srand(seed);
	
}


// The following two functions are only for multi-jittered sampling
// I had trouble getting them to compile in the MultiJittered class
// The overloading confused my compiler

// ---------------------------------------------------- rand_float with arguments

inline float 
RandomNumber::rand_float(int l, float h) {
	return (rand_float() * (h - l) + l);
}
//quick random function to distribute our initial points
inline float 
RandomNumber::rand_float_between(float min, float max)
{
	if(mersenne_twister)
	{    
		float r = (float)rand_int() / (float) MT_RAND_MAX;
		float ret = min + r * (max - min);
		return ret;
	}
	else
	{
		float r = (float)rand() / (float) RAND_MAX;
		return min + r * (max - min);
	}
}

// ---------------------------------------------------- rand_int with arguments

inline unsigned long
RandomNumber::rand_int(int l, int h) {
	return ((unsigned long) (rand_float(0, h - l + 1) + l));
	//return (int)rand_float_between((float)l, (float)h);
}


inline float
RandomNumber::getrand() 
{     
        float x = (rand_int() % 5 + 2.0f) / 10;		
        return x;
}





#endif
