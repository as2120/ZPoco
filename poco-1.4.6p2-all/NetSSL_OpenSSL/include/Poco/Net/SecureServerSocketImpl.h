﻿//
// SecureServerSocketImpl.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/SecureServerSocketImpl.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLSockets
// Module:  SecureServerSocketImpl
//
// Definition of the SecureServerSocketImpl class.
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


#ifndef NetSSL_SecureServerSocketImpl_INCLUDED
#define NetSSL_SecureServerSocketImpl_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/SecureSocketImpl.h"
#include "Poco/Net/ServerSocketImpl.h"
#include "Poco/Net/Context.h"


namespace Poco {
namespace Net {


class NetSSL_API SecureServerSocketImpl: public ServerSocketImpl
	/// The SocketImpl class for SecureServerSocket.
{
public:
	SecureServerSocketImpl(Context::Ptr pContext);
		/// Creates the SecureServerSocketImpl using the
		/// given SSL context object.

	SocketImpl* acceptConnection(SocketAddress& clientAddr);
		/// Get the next completed connection from the
		/// socket's completed connection queue.
		///
		/// If the queue is empty, waits until a connection
		/// request completes.
		///
		/// Returns a new TCP socket for the connection
		/// with the client.
		///
		/// The client socket's address is returned in clientAddr.
	
	void connect(const SocketAddress& address);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.

	void connect(const SocketAddress& address, const Poco::Timespan& timeout);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.

	void connectNB(const SocketAddress& address);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.
	
	void bind(const SocketAddress& address, bool reuseAddress = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		
	void listen(int backlog = 64);
		/// Puts the socket into listening state.
		///
		/// The socket becomes a passive socket that
		/// can accept incoming connection requests.
		///
		/// The backlog argument specifies the maximum
		/// number of connections that can be queued
		/// for this socket.

	void close();
		/// Close the socket.
	
	int sendBytes(const void* buffer, int length, int flags = 0);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.
	
	int receiveBytes(void* buffer, int length, int flags = 0);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.
	
	int sendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.
	
	int receiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.
	
	void sendUrgent(unsigned char data);
		/// Not supported by this kind of socket.
		///
		/// Throws a Poco::InvalidAccessException.

	bool secure() const;
		/// Returns true iff the socket's connection is secure
		/// (using SSL or TLS).
		
	Context::Ptr context() const;
		/// Returns the SSL context used by this socket.

protected:
	~SecureServerSocketImpl();
		/// Destroys the SecureServerSocketImpl.

private:
	SecureServerSocketImpl(const SecureServerSocketImpl&);
	SecureServerSocketImpl& operator = (const SecureServerSocketImpl&);

private:
	SecureSocketImpl _impl;
};


//
// inlines
//
inline Context::Ptr SecureServerSocketImpl::context() const
{
	return _impl.context();
}


} } // namespace Poco::Net


#endif // NetSSL_SecureServerSocketImpl_INCLUDED
