#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __REFERENCE_COUNT__
#define __REFERENCE_COUNT__

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


class ReferenceCount {
	public:
		ReferenceCount(void) 
			: references(0) { }

		void increment(void);

		int decrement(void);  
	  
	private:
		ReferenceCount(const ReferenceCount& rf);	// this function cannot be defined

		ReferenceCount&								// this function cannot be defined
		operator= (const ReferenceCount& rhs);

		int references;
};

inline void
ReferenceCount::increment(void) {
	references++;
}

inline int
ReferenceCount::decrement(void) {
	return (--references);
}

#endif