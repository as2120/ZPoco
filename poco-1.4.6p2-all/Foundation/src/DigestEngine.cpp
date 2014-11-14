﻿//
// DigestEngine.cpp
//
// $Id: //poco/1.4/Foundation/src/DigestEngine.cpp#1 $
//
// Library: Foundation
// Package: Crypt
// Module:  DigestEngine
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


#include "Poco/DigestEngine.h"


namespace Poco {


	DigestEngine::DigestEngine()
	{
	}


	DigestEngine::~DigestEngine()
	{
	}

	// byte to hex
	std::string DigestEngine::digestToHex(const Digest& bytes)
	{
		static const char digits[] = "0123456789abcdef";
		std::string result;
		result.reserve(bytes.size()*2);
		for (Digest::const_iterator it = bytes.begin(); it != bytes.end(); ++it)
		{
			unsigned char c = *it;
			result += digits[(c >> 4) & 0xF];
			result += digits[c & 0xF];
		}
		return result;
	}

} // namespace Poco