﻿//
// FunctionDelegate.h
//
// $Id: //poco/1.4/Foundation/include/Poco/FunctionDelegate.h#4 $
//
// Library: Foundation
// Package: Events
// Module:  FunctionDelegate
//
// Implementation of the FunctionDelegate template.
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


#ifndef Foundation_FunctionDelegate_INCLUDED
#define Foundation_FunctionDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class TArgs, bool hasSender = true, bool senderIsConst = true> 
class FunctionDelegate: public AbstractDelegate<TArgs>
	/// Wraps a freestanding function or static member function 
	/// for use as a Delegate.
{
public:
	typedef void (*NotifyMethod)(const void*, TArgs&);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(sender, arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}
	
	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


template <class TArgs> 
class FunctionDelegate<TArgs, true, false>: public AbstractDelegate<TArgs>
{
public:
	typedef void (*NotifyMethod)(void*, TArgs&);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(const_cast<void*>(sender), arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


template <class TArgs, bool senderIsConst> 
class FunctionDelegate<TArgs, false, senderIsConst>: public AbstractDelegate<TArgs>
{
public:
	typedef void (*NotifyMethod)(TArgs&);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


} // namespace Poco

#endif // Foundation_FunctionDelegate_INCLUDED
