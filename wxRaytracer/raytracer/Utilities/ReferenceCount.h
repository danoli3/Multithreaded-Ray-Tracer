#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __REFENCE_COUNTING__
#define __REFENCE_COUNTING__

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
    ReferenceCount() { references = 0; }
	void increment();
	int decrement();
    int references;
private:
    ReferenceCount( const ReferenceCount & );
    ReferenceCount &operator=( const ReferenceCount & );
};

inline void
ReferenceCount::increment()
{
	references++;
}

inline int
ReferenceCount::decrement()
{
	return (--references);
}


#endif