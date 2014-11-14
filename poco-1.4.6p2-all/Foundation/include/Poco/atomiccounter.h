﻿//
// AtomicCounter.h
//
// $Id: //poco/1.4/Foundation/include/Poco/AtomicCounter.h#8 $
//
// Library: Foundation
// Package: Core
// Module:  AtomicCounter
//
// Definition of the AtomicCounter class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_AtomicCounter_INCLUDED
#define Foundation_AtomicCounter_INCLUDED


#include "Poco/Foundation.h"
#if POCO_OS == POCO_OS_WINDOWS_NT
#include "Poco/UnWindows.h"
#elif POCO_OS == POCO_OS_MAC_OS_X
#include <libkern/OSAtomic.h>
#elif ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2) || __GNUC__ > 4) && (defined(__x86_64__) || defined(__i386__))
#if !defined(POCO_HAVE_GCC_ATOMICS) && !defined(POCO_NO_GCC_ATOMICS)
#define POCO_HAVE_GCC_ATOMICS
#endif
#elif ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) || __GNUC__ > 4)
#if !defined(POCO_HAVE_GCC_ATOMICS) && !defined(POCO_NO_GCC_ATOMICS)
#define POCO_HAVE_GCC_ATOMICS
#endif
#endif // POCO_OS
#include "Poco/Mutex.h"


namespace Poco {


class Foundation_API AtomicCounter
	//z 实现了一个简单的 counter， 提供了一个可在多线程环境下使用的 atomic 操作。
	/// This class implements a simple counter, which
	/// provides atomic operations that are safe to
	/// use in a multithreaded environment.
	///
	//z 原子计数器的典型使用场景是实现引用计数以及类似的事情
	/// Typical usage of AtomicCounter is for implementing
	/// reference counting and similar things.
	///
	//z 在一些平台上， AtomicCounter 的实现基于由系统提供的原子操作原语，因而效率非常高。
	/// On some platforms, the implementation of AtomicCounter
	/// is based on atomic primitives specific to the platform
	/// (such as InterlockedIncrement, etc. on Windows), and
	/// thus very efficient. On platforms that do not support
	/// atomic primitives, operations are guarded by a FastMutex.
	///
	/// The following platforms currently have atomic
	/// primitives:
	///   - Windows
	///   - Mac OS X
	///   - GCC 4.1+ (Intel platforms only)
{
public:
	//z 下面的 integer 类型
	typedef int ValueType; /// The underlying integer type.
	
	//z ctor ，初始化为0
	AtomicCounter();
		/// Creates a new AtomicCounter and initializes it to zero.
		
	//z ctor，初始化为给定的值
	explicit AtomicCounter(ValueType initialValue);
		/// Creates a new AtomicCounter and initializes it with
		/// the given value.
	
	//z copy ctor
	AtomicCounter(const AtomicCounter& counter);
		/// Creates the counter by copying another one.
	
	//z dtor
	~AtomicCounter();
		/// Destroys the AtomicCounter.

	//z 赋值操作，从另一个 AtomicCounter 获取值
	AtomicCounter& operator = (const AtomicCounter& counter);
		/// Assigns the value of another AtomicCounter.
		
	//z 从值类型赋值
	AtomicCounter& operator = (ValueType value);
		/// Assigns a value to the counter.

	//z 操作符，获取其值类型
	operator ValueType () const;
		/// Returns the value of the counter.
	
	//z 函数，获取值类型
	ValueType value() const;
		/// Returns the value of the counter.
		
	//z 前缀递增操作
	ValueType operator ++ (); // prefix
		/// Increments the counter and returns the result.
		
	//z 后缀递增操作
	ValueType operator ++ (int); // postfix
		/// Increments the counter and returns the previous value.
	
	//z 操作符，前缀递减
	ValueType operator -- (); // prefix
		/// Decrements the counter and returns the result.
		
	//z 操作符，后缀递减
	ValueType operator -- (int); // postfix
		/// Decrements the counter and returns the previous value.

	//z bool 操作符
	bool operator ! () const;
		/// Returns true if the counter is zero, false otherwise.

private:
#if POCO_OS == POCO_OS_WINDOWS_NT
	typedef volatile LONG ImplType;
#elif POCO_OS == POCO_OS_MAC_OS_X
	typedef int32_t ImplType;
#elif defined(POCO_HAVE_GCC_ATOMICS)
	typedef int ImplType;
#else // generic implementation based on FastMutex
	struct ImplType
	{
		mutable FastMutex mutex;
		volatile int      value;
	};
#endif // POCO_OS

	ImplType _counter;
};


//
// inlines
//


#if POCO_OS == POCO_OS_WINDOWS_NT
//
// Windows
//
//z 操作符，返回计数器的值
inline AtomicCounter::operator AtomicCounter::ValueType () const
{
	return _counter;
}


inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return InterlockedIncrement(&_counter);
}

inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	ValueType result = InterlockedIncrement(&_counter);
	return --result;
}

inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return InterlockedDecrement(&_counter);
}

inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	ValueType result = InterlockedDecrement(&_counter);
	return ++result;
}

inline bool AtomicCounter::operator ! () const
{
	return _counter == 0;
}


#elif POCO_OS == POCO_OS_MAC_OS_X
//
// Mac OS X
//
inline AtomicCounter::operator AtomicCounter::ValueType () const
{
	return _counter;
}

	
inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return OSAtomicIncrement32(&_counter);
}

	
inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	ValueType result = OSAtomicIncrement32(&_counter);
	return --result;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return OSAtomicDecrement32(&_counter);
}

	
inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	ValueType result = OSAtomicDecrement32(&_counter);
	return ++result;
}

	
inline bool AtomicCounter::operator ! () const
{
	return _counter == 0;
}

#elif defined(POCO_HAVE_GCC_ATOMICS)
//
// GCC 4.1+ atomic builtins.
//
inline AtomicCounter::operator AtomicCounter::ValueType () const
{
	return _counter;
}

	
inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return __sync_add_and_fetch(&_counter, 1);
}

	
inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	return __sync_fetch_and_add(&_counter, 1);
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return __sync_sub_and_fetch(&_counter, 1);
}

	
inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	return __sync_fetch_and_sub(&_counter, 1);
}

	
inline bool AtomicCounter::operator ! () const
{
	return _counter == 0;
}


#else
//
// Generic implementation based on FastMutex
//
inline AtomicCounter::operator AtomicCounter::ValueType () const
{
	ValueType result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = _counter.value;
	}
	return result;
}

	
inline AtomicCounter::ValueType AtomicCounter::value() const
{
	ValueType result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = _counter.value;
	}
	return result;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	ValueType result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = ++_counter.value;
	}
	return result;
}

	
inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	ValueType result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = _counter.value++;
	}
	return result;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	ValueType result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = --_counter.value;
	}
	return result;
}

	
inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	ValueType result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = _counter.value--;
	}
	return result;
}

	
inline bool AtomicCounter::operator ! () const
{
	bool result;
	{
		FastMutex::ScopedLock lock(_counter.mutex);
		result = _counter.value == 0;
	}
	return result;
}


#endif // POCO_OS


} // namespace Poco


#endif // Foundation_AtomicCounter_INCLUDED