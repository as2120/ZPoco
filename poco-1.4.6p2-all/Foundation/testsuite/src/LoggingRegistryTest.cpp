﻿//
// LoggingRegistryTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/LoggingRegistryTest.cpp#1 $
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


#include "LoggingRegistryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/AutoPtr.h"


using Poco::LoggingRegistry;
using Poco::Channel;
using Poco::ConsoleChannel;
using Poco::Formatter;
using Poco::PatternFormatter;
using Poco::AutoPtr;


LoggingRegistryTest::LoggingRegistryTest(const std::string& name): CppUnit::TestCase(name)
{
}


LoggingRegistryTest::~LoggingRegistryTest()
{
}


void LoggingRegistryTest::testRegister()
{
	LoggingRegistry& reg = LoggingRegistry::defaultRegistry();
	
	reg.clear();
	
	AutoPtr<Channel> pC1 = new ConsoleChannel();
	AutoPtr<Channel> pC2 = new ConsoleChannel();
	AutoPtr<Formatter> pF1 = new PatternFormatter("");
	AutoPtr<Formatter> pF2 = new PatternFormatter("");
	
	reg.registerChannel("c1", pC1);
	reg.registerChannel("c2", pC2);
	reg.registerFormatter("f1", pF1);
	reg.registerFormatter("f2", pF2);

	Channel* pC = reg.channelForName("c1");
	assert (pC1 == pC);
	pC = reg.channelForName("c2");
	assert (pC2 == pC);
	
	Formatter* pF = reg.formatterForName("f1");
	assert (pF1 == pF);
	pF = reg.formatterForName("f2");
	assert (pF2 == pF);
	
	try
	{
		pC = reg.channelForName("c3");
		fail("not found - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void LoggingRegistryTest::testReregister()
{
	LoggingRegistry& reg = LoggingRegistry::defaultRegistry();
	
	reg.clear();
	
	AutoPtr<Channel> pC1 = new ConsoleChannel();
	AutoPtr<Channel> pC2 = new ConsoleChannel();
	AutoPtr<Channel> pC1b = new ConsoleChannel();
	AutoPtr<Formatter> pF1 = new PatternFormatter("");
	AutoPtr<Formatter> pF2 = new PatternFormatter("");
	AutoPtr<Formatter> pF1b = new PatternFormatter("");
	
	reg.registerChannel("c1", pC1);
	reg.registerChannel("c2", pC2);
	reg.registerFormatter("f1", pF1);
	reg.registerFormatter("f2", pF2);
	
	reg.registerChannel("c1", pC1b);
	Channel* pC = reg.channelForName("c1");
	assert (pC1b == pC);
	pC = reg.channelForName("c2");
	assert (pC2 == pC);

	reg.registerFormatter("f1", pF1b);
	Formatter* pF = reg.formatterForName("f1");
	assert (pF1b == pF);
	pF = reg.formatterForName("f2");
	assert (pF2 == pF);
	
}


void LoggingRegistryTest::testUnregister()
{
	LoggingRegistry& reg = LoggingRegistry::defaultRegistry();
	
	reg.clear();
	
	AutoPtr<Channel> pC1 = new ConsoleChannel();
	AutoPtr<Channel> pC2 = new ConsoleChannel();
	AutoPtr<Formatter> pF1 = new PatternFormatter("");
	AutoPtr<Formatter> pF2 = new PatternFormatter("");
	
	reg.registerChannel("c1", pC1);
	reg.registerChannel("c2", pC2);
	reg.registerFormatter("f1", pF1);
	reg.registerFormatter("f2", pF2);

	reg.unregisterChannel("c1");
	reg.unregisterFormatter("f2");
	
	try
	{
		Channel* pC = reg.channelForName("c1");
		fail("unregistered - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}

	try
	{
		Formatter* pF = reg.formatterForName("f2");
		fail("unregistered - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void LoggingRegistryTest::setUp()
{
}


void LoggingRegistryTest::tearDown()
{
}


CppUnit::Test* LoggingRegistryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggingRegistryTest");

	CppUnit_addTest(pSuite, LoggingRegistryTest, testRegister);
	CppUnit_addTest(pSuite, LoggingRegistryTest, testReregister);
	CppUnit_addTest(pSuite, LoggingRegistryTest, testUnregister);

	return pSuite;
}