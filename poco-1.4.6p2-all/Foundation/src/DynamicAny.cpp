﻿//
// DynamicAny.cpp
//
// $Id: //poco/1.4/Foundation/src/DynamicAny.cpp#1 $
//
// Library: Foundation
// Package: Core
// Module:  DynamicAny
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#include "Poco/DynamicAny.h"
#include <algorithm>


namespace Poco {


DynamicAny::DynamicAny(): _pHolder(0)
{
}


DynamicAny::DynamicAny(const char* pVal): 
	_pHolder(new DynamicAnyHolderImpl<std::string>(pVal))
{
}


DynamicAny::DynamicAny(const DynamicAny& other):
	_pHolder(0)
{
	if (other._pHolder)
		_pHolder = other._pHolder->clone();
}


DynamicAny::~DynamicAny()
{
	delete _pHolder;
}


DynamicAny& DynamicAny::operator = (const DynamicAny& other)
{
	DynamicAny tmp(other);
	swap(tmp);
	return *this;
}


const DynamicAny DynamicAny::operator + (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return add<Poco::Int64>(other);
		else
			return add<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return add<double>(other);
	else if (isString())
		return add<std::string>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator += (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = add<Poco::Int64>(other);
		else
			return *this = add<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = add<double>(other);
	else if (isString())
		return *this = add<std::string>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


const DynamicAny DynamicAny::operator - (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return subtract<Poco::Int64>(other);
		else
			return subtract<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return subtract<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator -= (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = subtract<Poco::Int64>(other);
		else
			return *this = subtract<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = subtract<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


const DynamicAny DynamicAny::operator * (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return multiply<Poco::Int64>(other);
		else
			return multiply<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return multiply<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator *= (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = multiply<Poco::Int64>(other);
		else
			return *this = multiply<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = multiply<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


const DynamicAny DynamicAny::operator / (const DynamicAny& other) const
{
	if (isInteger())
	{
		if(isSigned())
			return divide<Poco::Int64>(other);
		else
			return divide<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return divide<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator /= (const DynamicAny& other)
{
	if (isInteger())
	{
		if(isSigned())
			return *this = divide<Poco::Int64>(other);
		else
			return *this = divide<Poco::UInt64>(other);
	}
	else if (isNumeric())
		return *this = divide<double>(other);
	else
		throw InvalidArgumentException("Invalid operation for this data type.");
}


DynamicAny& DynamicAny::operator ++ ()
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	return *this = *this + 1;
}

const DynamicAny DynamicAny::operator ++ (int)
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	DynamicAny tmp(*this);
	*this += 1;
	return tmp;
}

DynamicAny& DynamicAny::operator -- ()
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	return *this = *this - 1;
}

const DynamicAny DynamicAny::operator -- (int)
{
	if (!isInteger())
		throw InvalidArgumentException("Invalid operation for this data type.");

	DynamicAny tmp(*this);
	*this -= 1;
	return tmp;
}


bool DynamicAny::operator == (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<std::string>() == other.convert<std::string>();
}


bool DynamicAny::operator == (const char* other) const
{
	if (isEmpty()) return false;
	return convert<std::string>() == other;
}


bool DynamicAny::operator != (const DynamicAny& other) const
{
	if (isEmpty() && other.isEmpty()) return false;
	else if (isEmpty() || other.isEmpty()) return true;

	return convert<std::string>() != other.convert<std::string>();
}


bool DynamicAny::operator != (const char* other) const
{
	if (isEmpty()) return true;
	return convert<std::string>() != other;
}


bool DynamicAny::operator < (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<std::string>() < other.convert<std::string>();
}


bool DynamicAny::operator <= (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<std::string>() <= other.convert<std::string>();
}


bool DynamicAny::operator > (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<std::string>() > other.convert<std::string>();
}


bool DynamicAny::operator >= (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<std::string>() >= other.convert<std::string>();
}


bool DynamicAny::operator || (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<bool>() || other.convert<bool>();
}


bool DynamicAny::operator && (const DynamicAny& other) const
{
	if (isEmpty() || other.isEmpty()) return false;
	return convert<bool>() && other.convert<bool>();
}


void DynamicAny::empty()
{
	delete _pHolder;
	_pHolder = 0;
}


} // namespace Poco::Poco
