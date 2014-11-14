﻿//
// NetSSL.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/NetSSL.h#2 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  OpenSSL
//
// Basic definitions for the Poco OpenSSL library.
// This file must be the first file included by every other OpenSSL
// header file.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
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


#ifndef NetSSL_NetSSL_INCLUDED
#define NetSSL_NetSSL_INCLUDED


#include "Poco/Net/Net.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NetSSL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NetSSL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(POCO_DLL)
	#if defined(NetSSL_EXPORTS)
		#define NetSSL_API __declspec(dllexport)
	#else
		#define NetSSL_API __declspec(dllimport)
	#endif
#endif


#if !defined(NetSSL_API)
	#define NetSSL_API
#endif


//
// Automatically link NetSSL library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(NetSSL_EXPORTS)
		#pragma comment(lib, "PocoNetSSL" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco {
namespace Net {


void NetSSL_API initializeSSL();
	/// Initialize the NetSSL library, as well as the underlying OpenSSL
	/// libraries, by calling Poco::Crypto::OpenSSLInitializer::initialize().
	///
	/// Should be called before using any class from the NetSSL library.
	/// The NetSSL will be initialized automatically, through 
	/// Poco::Crypto::OpenSSLInitializer instances or similar mechanisms
	/// when creating Context or SSLManager instances.
	/// However, it is recommended to call initializeSSL()
	/// in any case at application startup.
	///
	/// Can be called multiple times; however, for every call to
	/// initializeSSL(), a matching call to uninitializeSSL()
	/// must be performed.
	

void NetSSL_API uninitializeSSL();
	/// Uninitializes the NetSSL library by calling 
	/// Poco::Crypto::OpenSSLInitializer::uninitialize() and
	/// shutting down the SSLManager.


} } // namespace Poco::Net


#endif // NetSSL_NetSSL_INCLUDED