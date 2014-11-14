﻿//
// AbstractPriorityDelegate.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AbstractPriorityDelegate.h#3 $
//
// Library: Foundation
// Package: Events
// Module:  AbstractPriorityDelegate
//
// Implementation of the AbstractPriorityDelegate template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_AbstractPriorityDelegate_INCLUDED
#define Foundation_AbstractPriorityDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"


namespace Poco {


template <class TArgs> 
class AbstractPriorityDelegate: public AbstractDelegate<TArgs>
	/// Base class for PriorityDelegate and PriorityExpire.
	///
	/// Extends AbstractDelegate with a priority value.
{
public:
	AbstractPriorityDelegate(int prio):
		_priority(prio)
	{
	}

	AbstractPriorityDelegate(const AbstractPriorityDelegate& del):
		AbstractDelegate<TArgs>(del),
		_priority(del._priority)
	{
	}

	virtual ~AbstractPriorityDelegate() 
	{
	}

	int priority() const
	{
		return _priority;
	}

protected:
	int _priority;
};


} // namespace Poco


#endif // Foundation_AbstractPriorityDelegate_INCLUDED