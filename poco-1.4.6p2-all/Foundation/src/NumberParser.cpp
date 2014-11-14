﻿//
// NumberParser.cpp
//
// $Id: //poco/1.4/Foundation/src/NumberParser.cpp#6 $
//
// Library: Foundation
// Package: Core
// Module:  NumberParser
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


#include "Poco/NumberParser.h"
#include "Poco/Exception.h"
#include "Poco/MemoryStream.h"
#if !defined(POCO_NO_LOCALE)
#include <locale>
#endif
#include <cstdio>


#if defined(POCO_LONG_IS_64_BIT)
	#define I64_FMT "l"
#elif defined(_MSC_VER) || defined(__MINGW32__ )
	#define I64_FMT "I64"
#elif defined(__APPLE__) 
	#define I64_FMT "q"
#else
	#define I64_FMT "ll"
#endif


namespace Poco {


int NumberParser::parse(const std::string& s)
{
	int result;
	if (tryParse(s, result))
		return result;
	else
		throw SyntaxException("Not a valid integer", s);
}


bool NumberParser::tryParse(const std::string& s, int& value)
{
	//z 这里的temp是用于存放字符串的结束符。
	char temp;
	return std::sscanf(s.c_str(), "%d%c", &value, &temp) == 1;
}


unsigned NumberParser::parseUnsigned(const std::string& s)
{
	unsigned result;
	if (tryParseUnsigned(s, result))
		return result;
	else
		throw SyntaxException("Not a valid unsigned integer", s);
}


bool NumberParser::tryParseUnsigned(const std::string& s, unsigned& value)
{
	char temp;
	return std::sscanf(s.c_str(), "%u%c", &value, &temp) == 1;
}


unsigned NumberParser::parseHex(const std::string& s)
{
	unsigned result;
	if (tryParseHex(s, result))
		return result;
	else
		throw SyntaxException("Not a valid hexadecimal integer", s);
}


bool NumberParser::tryParseHex(const std::string& s, unsigned& value)
{
	char temp;
	return std::sscanf(s.c_str(), "%x%c", &value, &temp) == 1;
}


#if defined(POCO_HAVE_INT64)


Int64 NumberParser::parse64(const std::string& s)
{
	Int64 result;
	if (tryParse64(s, result))
		return result;
	else
		throw SyntaxException("Not a valid integer", s);
}


bool NumberParser::tryParse64(const std::string& s, Int64& value)
{
	char temp;
	return std::sscanf(s.c_str(), "%" I64_FMT "d%c", &value, &temp) == 1;
}


UInt64 NumberParser::parseUnsigned64(const std::string& s)
{
	UInt64 result;
	if (tryParseUnsigned64(s, result))
		return result;
	else
		throw SyntaxException("Not a valid unsigned integer", s);
}


bool NumberParser::tryParseUnsigned64(const std::string& s, UInt64& value)
{
	char temp;
	return std::sscanf(s.c_str(), "%" I64_FMT "u%c", &value, &temp) == 1;
}


UInt64 NumberParser::parseHex64(const std::string& s)
{
	UInt64 result;
	if (tryParseHex64(s, result))
		return result;
	else
		throw SyntaxException("Not a valid hexadecimal integer", s);
}


bool NumberParser::tryParseHex64(const std::string& s, UInt64& value)
{
	char temp;
	return std::sscanf(s.c_str(), "%" I64_FMT "x%c", &value, &temp) == 1;
}


#endif // defined(POCO_HAVE_INT64)


double NumberParser::parseFloat(const std::string& s)
{
	double result;
	if (tryParseFloat(s, result))
		return result;
	else
		throw SyntaxException("Not a valid floating-point number", s);
}

	
bool NumberParser::tryParseFloat(const std::string& s, double& value)
{
	Poco::MemoryInputStream istr(s.data(), s.size());
#if !defined(POCO_NO_LOCALE)
	istr.imbue(std::locale::classic());
#endif
	istr >> value;
	return istr.eof() && !istr.fail();
}


} // namespace Poco
