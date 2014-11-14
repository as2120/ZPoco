﻿//
// UnufferedStreamBuf.h
//
// $Id: //poco/1.4/Foundation/include/Poco/UnbufferedStreamBuf.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  StreamBuf
//
// Definition of template BasicUnbufferedStreamBuf and class UnbufferedStreamBuf.
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


#ifndef Foundation_UnbufferedStreamBuf_INCLUDED
#define Foundation_UnbufferedStreamBuf_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/StreamUtil.h"
#include <streambuf>
#include <iosfwd>
#include <ios>


namespace Poco {


template <typename ch, typename tr> 
class BasicUnbufferedStreamBuf: public std::basic_streambuf<ch, tr>
	/// This is an implementation of an unbuffered streambuf
	/// that greatly simplifies the implementation of
	/// custom streambufs of various kinds.
	/// Derived classes only have to override the methods
	/// readFromDevice() or writeToDevice().
{
protected:
	typedef std::basic_streambuf<ch, tr> Base;
	typedef std::basic_ios<ch, tr> IOS;
	typedef ch char_type;
	typedef tr char_traits;
	typedef typename Base::int_type int_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::off_type off_type;
	typedef typename IOS::openmode openmode;

public:
	BasicUnbufferedStreamBuf():
		_pb(char_traits::eof()),
		_ispb(false)
	{
		this->setg(0, 0, 0);
		this->setp(0, 0);
	}

	~BasicUnbufferedStreamBuf()
	{
	}

	virtual int_type overflow(int_type c)
	{
		if (c != char_traits::eof()) 
			return writeToDevice(char_traits::to_char_type(c));
		else
			return c;
	}

	virtual int_type underflow()
	{
		if (_ispb)
		{
    		return _pb;
		}
		else
		{
    		int_type c = readFromDevice();
    		if (c != char_traits::eof())
    		{
    			_ispb = true;
    			_pb   = c;
    		}
    		return c;
		}
	}

	virtual int_type uflow()
	{
		if (_ispb)
		{
			_ispb = false;
			return _pb;
		}
		else
		{
    		int_type c = readFromDevice();
    		if (c != char_traits::eof())
    		{
    			_pb = c;
    		}
    		return c;
		}
	}

	virtual int_type pbackfail(int_type c)
	{
		if (_ispb)
		{
    		return char_traits::eof();
		}
		else
		{
    		_ispb = true;
    		_pb   = c;
    		return c;
		}
	}
	
	virtual std::streamsize xsgetn(char_type* p, std::streamsize count)
		/// Some platforms (for example, Compaq C++) have buggy implementations of 
		/// xsgetn that handle null buffers incorrectly.
		/// Anyway, it does not hurt to provide an optimized implementation
		/// of xsgetn for this streambuf implementation.
	{
		std::streamsize copied = 0;
		while (count > 0)
		{
			int_type c = uflow();
			if (c == char_traits::eof()) break;
			*p++ = char_traits::to_char_type(c);
			++copied;
			--count;
		}
		return copied;
	}

protected:
	static int_type charToInt(char_type c)
	{
		return char_traits::to_int_type(c);
	}

private:
	virtual int_type readFromDevice()
	{
		return char_traits::eof();
	}
	
	virtual int_type writeToDevice(char_type)
	{
		return char_traits::eof();
	}

	int_type _pb;
	bool     _ispb;
	
	BasicUnbufferedStreamBuf(const BasicUnbufferedStreamBuf&);
	BasicUnbufferedStreamBuf& operator = (const BasicUnbufferedStreamBuf&);
};


//
// We provide an instantiation for char
//
typedef BasicUnbufferedStreamBuf<char, std::char_traits<char> > UnbufferedStreamBuf;


} // namespace Poco


#endif // Foundation_UnbufferedStreamBuf_INCLUDED
