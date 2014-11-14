﻿//
// TextConverter.h
//
// $Id: //poco/1.4/Foundation/include/Poco/TextConverter.h#1 $
//
// Library: Foundation
// Package: Text
// Module:  TextConverter
//
// Definition of the TextConverter class.
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


#ifndef Foundation_TextConverter_INCLUDED
#define Foundation_TextConverter_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class TextEncoding;


class Foundation_API TextConverter
	/// A TextConverter converts strings from one encoding
	/// into another.
{
public:
	typedef int (*Transform)(int);
		/// Transform function for convert.
		
	TextConverter(const TextEncoding& inEncoding, const TextEncoding& outEncoding, int defaultChar = '?');
		/// Creates the TextConverter. The encoding objects must not be deleted while the
		/// TextConverter is in use.

	~TextConverter();
		/// Destroys the TextConverter.
		
	int convert(const std::string& source, std::string& destination, Transform trans);
		/// Converts the source string from inEncoding to outEncoding
		/// and appends the result to destination. Every character is
		/// passed to the transform function.
		/// If a character cannot be represented in outEncoding, defaultChar
		/// is used instead.
		/// Returns the number of encoding errors (invalid byte sequences
		/// in source).

	int convert(const void* source, int length, std::string& destination, Transform trans);
		/// Converts the source buffer from inEncoding to outEncoding
		/// and appends the result to destination. Every character is
		/// passed to the transform function.
		/// If a character cannot be represented in outEncoding, defaultChar
		/// is used instead.
		/// Returns the number of encoding errors (invalid byte sequences
		/// in source).

	int convert(const std::string& source, std::string& destination);
		/// Converts the source string from inEncoding to outEncoding
		/// and appends the result to destination.
		/// If a character cannot be represented in outEncoding, defaultChar
		/// is used instead.
		/// Returns the number of encoding errors (invalid byte sequences
		/// in source).

	int convert(const void* source, int length, std::string& destination);
		/// Converts the source buffer from inEncoding to outEncoding
		/// and appends the result to destination.
		/// If a character cannot be represented in outEncoding, defaultChar
		/// is used instead.
		/// Returns the number of encoding errors (invalid byte sequences
		/// in source).

private:
	TextConverter();
	TextConverter(const TextConverter&);
	TextConverter& operator = (const TextConverter&);

	const TextEncoding& _inEncoding;
	const TextEncoding& _outEncoding;
	int                 _defaultChar;
};


} // namespace Poco


#endif // Foundation_TextConverter_INCLUDED
