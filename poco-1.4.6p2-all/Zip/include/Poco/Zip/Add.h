﻿//
// Add.h
//
// $Id: //poco/1.4/Zip/include/Poco/Zip/Add.h#1 $
//
// Library: Zip
// Package: Manipulation
// Module:  Add
//
// Definition of the Add class.
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


#ifndef Zip_Add_INCLUDED
#define Zip_Add_INCLUDED


#include "Poco/Zip/Zip.h"
#include "Poco/Zip/ZipOperation.h"
#include "Poco/Zip/ZipCommon.h"


namespace Poco {
namespace Zip {


class Zip_API Add: public ZipOperation
	/// Operation Add adds a new file entry to an existing Zip File
{
public:
	Add(const std::string& zipPath, const std::string& localPath, ZipCommon::CompressionMethod cm, ZipCommon::CompressionLevel cl);
		/// Creates the Add.

	void execute(Compress& c, std::istream& input);
		/// Performs the add operation

private:
	const std::string _zipPath;
	const std::string _localPath;
	const ZipCommon::CompressionMethod _cm;
	const ZipCommon::CompressionLevel _cl;
};


} } // namespace Poco::Zip


#endif // Zip_Add_INCLUDED
