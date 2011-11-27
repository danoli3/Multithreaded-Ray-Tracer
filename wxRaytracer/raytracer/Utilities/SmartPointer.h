#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SMART_POINTER__
#define __SMART_POINTER__

#include "ReferenceCount.h"

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

// SmartPointer
template <typename T> class SmartPointer {
public:
    // SmartPointer Public Methods
    SmartPointer(T *ptr = NULL) {
        pointer = ptr;
        if (pointer) 
			pointer->increment();
    }

    SmartPointer(const SmartPointer<T> &r)
	{   
		pointer = r.pointer;
		if (pointer)
			pointer->increment();			

    }

    SmartPointer &operator=(const SmartPointer<T> &r)
	{   if (r.pointer) 
			r.pointer->increment();
        if (pointer && pointer->decrement() == 0)
		{	delete pointer;
			pointer = NULL;
		}
        pointer = r.pointer;
        return *this;
    }

    SmartPointer &operator=(T *ptr)
	{	if (ptr)
			ptr->increment();
        if (pointer && pointer->decrement() == 0)
		{	delete pointer;
			pointer = NULL;
		}
        pointer = ptr;
        return *this;
    }

    ~SmartPointer() {
        if (pointer && pointer->decrement() == 0)
        {    delete pointer;
			 pointer = NULL;
		}
    }

	void Delete();

    T *operator->()
	{ return pointer; }	

    const T *operator->() const
	{ return pointer; }

    operator bool() const
	{ return pointer != NULL; }

    const T *Pointer() const
	{ return pointer; }

	// Becareful with this one... full access to delete the pointer if not careful
	T *Get() const
	{ return pointer; }



private:
    T *pointer;
};

template <typename T>
inline void
SmartPointer<T>::Delete()
{
    if (pointer && pointer->decrement() == 0)
    {    delete pointer;
		 pointer = NULL;
	}
}

#endif