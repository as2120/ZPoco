﻿//
// SMTPClientSessionTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/SMTPClientSessionTest.cpp#1 $
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


#include "SMTPClientSessionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "DialogServer.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/NetException.h"


using Poco::Net::SMTPClientSession;
using Poco::Net::MailMessage;
using Poco::Net::MailRecipient;
using Poco::Net::SMTPException;


SMTPClientSessionTest::SMTPClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


SMTPClientSessionTest::~SMTPClientSessionTest()
{
}


void SMTPClientSessionTest::testLoginEHLO()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("221 Bye");
	SMTPClientSession session("localhost", server.port());
	session.login("localhost");
	std::string cmd = server.popCommand();
	assert (cmd == "EHLO localhost");
	session.close();
	cmd = server.popCommand();
	assert (cmd == "QUIT");
}


void SMTPClientSessionTest::testLoginHELO()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("500 EHLO not understood");
	server.addResponse("250 Hello localhost");
	server.addResponse("221 Bye");
	SMTPClientSession session("localhost", server.port());
	session.login("localhost");
	std::string cmd = server.popCommand();
	assert (cmd == "EHLO localhost");
	cmd = server.popCommand();
	assert (cmd == "HELO localhost");
	session.close();
	cmd = server.popCommand();
	assert (cmd == "QUIT");
}


void SMTPClientSessionTest::testLoginFailed()
{
	DialogServer server;
	server.addResponse("500 No SMTP service here");
	server.addResponse("221 Bye");
	SMTPClientSession session("localhost", server.port());
	try
	{
		session.login("localhost");
		fail("cannot login - must throw");
	}
	catch (SMTPException&)
	{
	}
	session.close();
}


void SMTPClientSessionTest::testSend()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("354 Send data");
	server.addResponse("250 OK");
	server.addResponse("221 Bye");
	SMTPClientSession session("localhost", server.port());
	session.login("localhost");

	MailMessage message;
	message.setSender("john.doe@no.where");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe"));
	message.setSubject("Test Message");
	message.setContent("Hello\r\nblah blah\r\n\r\nJohn\r\n");
	server.clearCommands();
	session.sendMessage(message);
	std::string cmd = server.popCommandWait();
	assert (cmd == "MAIL FROM: <john.doe@no.where>");
	cmd = server.popCommandWait();
	assert (cmd == "RCPT TO: <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assert (cmd == "DATA");
	cmd = server.popCommandWait();
	assert (cmd == "Content-Transfer-Encoding: quoted-printable");
	cmd = server.popCommandWait();
	assert (cmd == "Content-Type: text/plain");
	cmd = server.popCommandWait();
	assert (cmd.substr(0, 4) == "Date");
	cmd = server.popCommandWait();
	assert (cmd == "From: john.doe@no.where");
	cmd = server.popCommandWait();
	assert (cmd == "Subject: Test Message");
	cmd = server.popCommandWait();
	assert (cmd == "To: Jane Doe <jane.doe@no.where>");
	cmd = server.popCommandWait();
	assert (cmd == "Hello");
	cmd = server.popCommandWait();
	assert (cmd == "blah blah");
	cmd = server.popCommandWait();
	assert (cmd == "John");
	cmd = server.popCommandWait();
	assert (cmd == ".");

	session.close();
}


void SMTPClientSessionTest::testSendFailed()
{
	DialogServer server;
	server.addResponse("220 localhost SMTP ready");
	server.addResponse("250 Hello localhost");
	server.addResponse("250 OK");
	server.addResponse("250 OK");
	server.addResponse("354 Send data");
	server.addResponse("500 Error");
	server.addResponse("221 Bye");
	SMTPClientSession session("localhost", server.port());
	session.login("localhost");

	MailMessage message;
	message.setSender("john.doe@no.where");
	message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, "jane.doe@no.where", "Jane Doe"));
	message.setSubject("Test Message");
	message.setContent("Hello\r\nblah blah\r\n\r\nJohn\r\n");
	server.clearCommands();
	try
	{
		session.sendMessage(message);
		fail("internal error - must throw");
	}
	catch (SMTPException&)
	{
	}
	
	session.close();
}


void SMTPClientSessionTest::setUp()
{
}


void SMTPClientSessionTest::tearDown()
{
}


CppUnit::Test* SMTPClientSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SMTPClientSessionTest");

	CppUnit_addTest(pSuite, SMTPClientSessionTest, testLoginEHLO);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testLoginHELO);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testLoginFailed);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testSend);
	CppUnit_addTest(pSuite, SMTPClientSessionTest, testSendFailed);

	return pSuite;
}
