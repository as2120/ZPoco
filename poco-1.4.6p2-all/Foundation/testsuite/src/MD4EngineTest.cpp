﻿//
// MD4EngineTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/MD4EngineTest.cpp#1 $
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


#include "MD4EngineTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MD4Engine.h"


using Poco::MD4Engine;
using Poco::DigestEngine;


MD4EngineTest::MD4EngineTest(const std::string& name): CppUnit::TestCase(name)
{
}


MD4EngineTest::~MD4EngineTest()
{
}


void MD4EngineTest::testMD4()
{
	MD4Engine engine;

	// test vectors from RFC 1320

	engine.update("");
	assert (DigestEngine::digestToHex(engine.digest()) == "31d6cfe0d16ae931b73c59d7e0c089c0");

	engine.update("a");
	assert (DigestEngine::digestToHex(engine.digest()) == "bde52cb31de33e46245e05fbdbd6fb24");

	engine.update("abc");
	assert (DigestEngine::digestToHex(engine.digest()) == "a448017aaf21d8525fc10ae87aa6729d");

	engine.update("message digest");
	assert (DigestEngine::digestToHex(engine.digest()) == "d9130a8164549fe818874806e1c7014b");

	engine.update("abcdefghijklmnopqrstuvwxyz");
	assert (DigestEngine::digestToHex(engine.digest()) == "d79e1c308aa5bbcdeea8ed63df412da9");
	
	engine.update("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	engine.update("abcdefghijklmnopqrstuvwxyz0123456789");
	assert (DigestEngine::digestToHex(engine.digest()) == "043f8582f241db351ce627e153e7f0e4");

	engine.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	assert (DigestEngine::digestToHex(engine.digest()) == "e33b4ddc9c38f2199c3e7b164fcc0536");
}


void MD4EngineTest::setUp()
{
}


void MD4EngineTest::tearDown()
{
}


CppUnit::Test* MD4EngineTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("MD4EngineTest");

	CppUnit_addTest(pSuite, MD4EngineTest, testMD4);

	return pSuite;
}
