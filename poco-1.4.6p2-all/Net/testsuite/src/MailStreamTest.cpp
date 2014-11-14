﻿//
// MailStreamTest.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/MailStreamTest.cpp#2 $
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


#include "MailStreamTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/MailStream.h"
#include "Poco/StreamCopier.h"
#include <sstream>


using Poco::Net::MailInputStream;
using Poco::Net::MailOutputStream;
using Poco::StreamCopier;


MailStreamTest::MailStreamTest(const std::string& name): CppUnit::TestCase(name)
{
}


MailStreamTest::~MailStreamTest()
{
}


void MailStreamTest::testMailInputStream()
{
	std::istringstream istr(
		"From: john.doe@no.domain\r\n"
		"To: jane.doe@no.domain\r\n"
		"Subject: test\r\n"
		"\r\n"
		"This is a test.\r\n"
		"\rThis.is.\ngarbage\r.\r\n"
		".This line starts with a period.\r\n"
		"..and this one too\r\n"
		"..\r\n"
		".\r\n"
	);
	
	MailInputStream mis(istr);
	std::ostringstream ostr;
	StreamCopier::copyStream(mis, ostr);
	std::string s(ostr.str());
	assert (s ==
		"From: john.doe@no.domain\r\n"
		"To: jane.doe@no.domain\r\n"
		"Subject: test\r\n"
		"\r\n"
		"This is a test.\r\n"
		"\rThis.is.\ngarbage\r.\r\n"
		".This line starts with a period.\r\n"
		".and this one too\r\n"
		".\r\n"
	);	
}


void MailStreamTest::testMailOutputStream()
{
	std::string msg(
		"From: john.doe@no.domain\r\n"
		"To: jane.doe@no.domain\r\n"
		"Subject: test\r\n"
		"\r\n"
		"This is a test.\r\n"
		"\rThis.is.\ngarbage\r.\r\n"
		".This line starts with a period.\r\n"
		"\r\n"
		".and this one too\r\n"
		".\r\n"
	);
	
	std::ostringstream ostr;
	MailOutputStream mos(ostr);
	mos << msg;
	mos.close();
	std::string s(ostr.str());
	assert (s == 
		"From: john.doe@no.domain\r\n"
		"To: jane.doe@no.domain\r\n"
		"Subject: test\r\n"
		"\r\n"
		"This is a test.\r\n"
		"\rThis.is.\ngarbage\r.\r\n"
		"..This line starts with a period.\r\n"
		"\r\n"
		"..and this one too\r\n"
		"..\r\n"
		".\r\n"
	);
}


void MailStreamTest::setUp()
{
}


void MailStreamTest::tearDown()
{
}


CppUnit::Test* MailStreamTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MailStreamTest");

	CppUnit_addTest(pSuite, MailStreamTest, testMailInputStream);
	CppUnit_addTest(pSuite, MailStreamTest, testMailOutputStream);

	return pSuite;
}