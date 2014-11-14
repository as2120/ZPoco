﻿//
// AutoReleasePool.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AutoReleasePool.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  AutoReleasePool
//
// Definition of the AutoReleasePool class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_AutoReleasePool_INCLUDED
#define Foundation_AutoReleasePool_INCLUDED


#include "Poco/Foundation.h"
#include <list>


namespace Poco {


template <class C>
class AutoReleasePool
	/// An AutoReleasePool implements simple garbage collection for 
	/// reference-counted objects.
	/// It temporarily takes ownwership of reference-counted objects that 
	/// nobody else wants to take ownership of and releases them 
	/// at a later, appropriate point in time.
	///
	/// Note: The correct way to add an object hold by an AutoPtr<> to
	/// an AutoReleasePool is by invoking the AutoPtr's duplicate()
	/// method. Example:
	///    AutoReleasePool<C> arp;
	///    AutoPtr<C> ptr = new C;
	///    ...
	///    arp.add(ptr.duplicate());
{
public:
	AutoReleasePool()
		/// Creates the AutoReleasePool.
	{
	}
	
	~AutoReleasePool()
		/// Destroys the AutoReleasePool and releases
		/// all objects it currently holds.
	{
		release();
	}
	
	void add(C* pObject)
		/// Adds the given object to the AutoReleasePool.
		/// The object's reference count is not modified
	{
		if (pObject)
			_list.push_back(pObject);
	}
	
	void release()
		/// Releases all objects the AutoReleasePool currently holds
		/// by calling each object's release() method.
	{
		while (!_list.empty())
		{
			_list.front()->release();
			_list.pop_front();
		}
	}
	
private:
	typedef std::list<C*> ObjectList;
	
	ObjectList _list;
};


} // namespace Poco


#endif // Foundation_AutoReleasePool_INCLUDED
