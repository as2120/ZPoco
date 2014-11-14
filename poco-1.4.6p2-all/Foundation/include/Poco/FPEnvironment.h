﻿//
// FPEnvironment.h
//
// $Id: //poco/1.4/Foundation/include/Poco/FPEnvironment.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  FPEnvironment
//
// Definitions of class FPEnvironment.
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


#ifndef Foundation_FPEnvironment_INCLUDED
#define Foundation_FPEnvironment_INCLUDED


#include "Poco/Foundation.h"


#if defined(POCO_NO_FPENVIRONMENT)
#include "Poco/FPEnvironment_DUMMY.h"
#elif defined(__osf__) || defined(__VMS)
#include "Poco/FPEnvironment_DEC.h"
#elif defined(sun) || defined(__sun)
#include "Poco/FPEnvironment_SUN.h"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "Poco/FPEnvironment_C99.h"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/FPEnvironment_WIN32.h"
#else
#include "Poco/FPEnvironment_DUMMY.h"
#endif


namespace Poco {


class Foundation_API FPEnvironment: private FPEnvironmentImpl
	/// Instances of this class can be used to save
	/// and later restore the current floating
	/// point environment (consisting of rounding
	/// mode and floating-point flags).
	/// The class also provides various static
	/// methods to query certain properties
	/// of a floating-point number. 
{
public:
	enum RoundingMode
	{
		FP_ROUND_DOWNWARD   = FP_ROUND_DOWNWARD_IMPL,
		FP_ROUND_UPWARD     = FP_ROUND_UPWARD_IMPL,
		FP_ROUND_TONEAREST  = FP_ROUND_TONEAREST_IMPL,
		FP_ROUND_TOWARDZERO = FP_ROUND_TOWARDZERO_IMPL
	};

	enum Flag
	{
		FP_DIVIDE_BY_ZERO = FP_DIVIDE_BY_ZERO_IMPL,
		FP_INEXACT        = FP_INEXACT_IMPL,
		FP_OVERFLOW       = FP_OVERFLOW_IMPL,
		FP_UNDERFLOW      = FP_UNDERFLOW_IMPL,
		FP_INVALID        = FP_INVALID_IMPL
	};

	FPEnvironment();
		/// Standard constructor.
		/// Remembers the current environment.

	FPEnvironment(RoundingMode mode);
		/// Remembers the current environment and
		/// sets the given rounding mode.
		
	FPEnvironment(const FPEnvironment& env);
		/// Copy constructor.
		
	~FPEnvironment();
		/// Restores the previous environment (unless
		/// keepCurrent() has been called previously)
		
	FPEnvironment& operator = (const FPEnvironment& env);
		/// Assignment operator

	void keepCurrent();
		/// Keep the current environment even after
		/// destroying the FPEnvironment object.

	static void clearFlags();
		/// Resets all flags.

	static bool isFlag(Flag flag);
		/// Returns true iff the given flag is set.
		
	static void setRoundingMode(RoundingMode mode);
		/// Sets the rounding mode.
		
	static RoundingMode getRoundingMode();
		/// Returns the current rounding mode.
		
	static bool isInfinite(float value);		
	static bool isInfinite(double value);
	static bool isInfinite(long double value);
		/// Returns true iff the given number is infinite.

	static bool isNaN(float value);		
	static bool isNaN(double value);
	static bool isNaN(long double value);
		/// Returns true iff the given number is NaN.

	static float copySign(float target, float source);		
	static double copySign(double target, double source);
	static long double copySign(long double target, long double source);
		/// Copies the sign from source to target.
};


//
// For convenience, we provide a shorter name for
// the FPEnvironment class.
//
typedef FPEnvironment FPE;


//
// inline's
//
inline bool FPEnvironment::isFlag(Flag flag)
{
	return isFlagImpl(FlagImpl(flag));
}


inline void FPEnvironment::setRoundingMode(RoundingMode mode)
{
	setRoundingModeImpl(RoundingModeImpl(mode));
}

	
inline FPEnvironment::RoundingMode FPEnvironment::getRoundingMode()
{
	return RoundingMode(getRoundingModeImpl());
}

	
inline bool FPEnvironment::isInfinite(float value)
{
	return isInfiniteImpl(value);
}


inline bool FPEnvironment::isInfinite(double value)
{
	return isInfiniteImpl(value);
}


inline bool FPEnvironment::isInfinite(long double value)
{
	return isInfiniteImpl(value);
}


inline bool FPEnvironment::isNaN(float value)
{
	return isNaNImpl(value);
}


inline bool FPEnvironment::isNaN(double value)
{
	return isNaNImpl(value);
}


inline bool FPEnvironment::isNaN(long double value)
{
	return isNaNImpl(value);
}


inline float FPEnvironment::copySign(float target, float source)
{
	return copySignImpl(target, source);
}


inline double FPEnvironment::copySign(double target, double source)
{
	return copySignImpl(target, source);
}


inline long double FPEnvironment::copySign(long double target, long double source)
{
	return copySignImpl(target, source);
}


} // namespace Poco


#endif // Foundation_FPEnvironment_INCLUDED
