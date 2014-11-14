﻿//
// HTTPChunkedStream.cpp
//
// $Id: //poco/1.4/Net/src/HTTPChunkedStream.cpp#1 $
//
// Library: Net
// Package: HTTP
// Module:  HTTPChunkedStream
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/Ascii.h"


using Poco::NumberFormatter;
using Poco::NumberParser;


namespace Poco {
namespace Net {


//
// HTTPChunkedStreamBuf
//


HTTPChunkedStreamBuf::HTTPChunkedStreamBuf(HTTPSession& session, openmode mode):
	HTTPBasicStreamBuf(HTTPBufferAllocator::BUFFER_SIZE, mode),
	_session(session),
	_mode(mode),
	_chunk(0)
{
}


HTTPChunkedStreamBuf::~HTTPChunkedStreamBuf()
{
}


void HTTPChunkedStreamBuf::close()
{
	if (_mode & std::ios::out)
	{
		sync();
		_session.write("0\r\n\r\n", 5);
	}
}


int HTTPChunkedStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	static const int eof = std::char_traits<char>::eof();

	if (_chunk == 0)
	{
		int ch = _session.get();
		while (Poco::Ascii::isSpace(ch)) ch = _session.get();
		std::string chunkLen;
		while (Poco::Ascii::isHexDigit(ch)) { chunkLen += (char) ch; ch = _session.get(); }
		while (ch != eof && ch != '\n') ch = _session.get();
		unsigned chunk;
		if (NumberParser::tryParseHex(chunkLen, chunk))
			_chunk = (std::streamsize) chunk;
		else
			return eof;
	}
	if (_chunk > 0)
	{
		if (length > _chunk) length = _chunk;
		int n = _session.read(buffer, length);
		if (n > 0) _chunk -= n;
		return n;
	}
	else 
	{
		int ch = _session.get();
		while (ch != eof && ch != '\n') ch = _session.get();
		return 0;
	}
}


int HTTPChunkedStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	_chunkBuffer.clear();
	NumberFormatter::appendHex(_chunkBuffer, length);
	_chunkBuffer.append("\r\n", 2);
	_chunkBuffer.append(buffer, static_cast<std::string::size_type>(length));
	_chunkBuffer.append("\r\n", 2);
	_session.write(_chunkBuffer.data(), static_cast<std::streamsize>(_chunkBuffer.size()));
	return static_cast<int>(length);
}


//
// HTTPChunkedIOS
//


HTTPChunkedIOS::HTTPChunkedIOS(HTTPSession& session, HTTPChunkedStreamBuf::openmode mode):
	_buf(session, mode)
{
	poco_ios_init(&_buf);
}


HTTPChunkedIOS::~HTTPChunkedIOS()
{
	try
	{
		_buf.close();
	}
	catch (...)
	{
	}
}


HTTPChunkedStreamBuf* HTTPChunkedIOS::rdbuf()
{
	return &_buf;
}


//
// HTTPChunkedInputStream
//


Poco::MemoryPool HTTPChunkedInputStream::_pool(sizeof(HTTPChunkedInputStream));


HTTPChunkedInputStream::HTTPChunkedInputStream(HTTPSession& session):
	HTTPChunkedIOS(session, std::ios::in),
	std::istream(&_buf)
{
}


HTTPChunkedInputStream::~HTTPChunkedInputStream()
{
}


void* HTTPChunkedInputStream::operator new(std::size_t size)
{
	return _pool.get();
}


void HTTPChunkedInputStream::operator delete(void* ptr)
{
	_pool.release(ptr);
}


//
// HTTPChunkedOutputStream
//


Poco::MemoryPool HTTPChunkedOutputStream::_pool(sizeof(HTTPChunkedOutputStream));


HTTPChunkedOutputStream::HTTPChunkedOutputStream(HTTPSession& session):
	HTTPChunkedIOS(session, std::ios::out),
	std::ostream(&_buf)
{
}


HTTPChunkedOutputStream::~HTTPChunkedOutputStream()
{
}


void* HTTPChunkedOutputStream::operator new(std::size_t size)
{
	return _pool.get();
}


void HTTPChunkedOutputStream::operator delete(void* ptr)
{
	_pool.release(ptr);
}


} } // namespace Poco::Net
