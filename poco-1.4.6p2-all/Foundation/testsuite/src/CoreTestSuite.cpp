﻿//
// CoreTestSuite.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/CoreTestSuite.cpp#3 $
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


#include "CoreTestSuite.h"
#include "CoreTest.h"
#include "AutoPtrTest.h"
#include "SharedPtrTest.h"
#include "AutoReleasePoolTest.h"
#include "ByteOrderTest.h"
#include "StringTest.h"
#include "StringTokenizerTest.h"
#ifndef POCO_VXWORKS
#include "FPETest.h"
#endif
#include "RegularExpressionTest.h"
#include "NDCTest.h"
#include "NumberFormatterTest.h"
#include "NumberParserTest.h"
#include "DynamicFactoryTest.h"
#include "MemoryPoolTest.h"
#include "AnyTest.h"
#include "DynamicAnyTest.h"
#include "FormatTest.h"
#include "TuplesTest.h"
#ifndef POCO_VXWORKS
#include "NamedTuplesTest.h"
#endif
#include "TypeListTest.h"
#include "ObjectPoolTest.h"


CppUnit::Test* CoreTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("CoreTestSuite");

	pSuite->addTest(CoreTest::suite());
	pSuite->addTest(AutoPtrTest::suite());
	pSuite->addTest(SharedPtrTest::suite());
	pSuite->addTest(AutoReleasePoolTest::suite());
	pSuite->addTest(ByteOrderTest::suite());
	pSuite->addTest(StringTest::suite());
	pSuite->addTest(StringTokenizerTest::suite());
#ifndef POCO_VXWORKS
	pSuite->addTest(FPETest::suite());
#endif
	pSuite->addTest(RegularExpressionTest::suite());
	pSuite->addTest(NDCTest::suite());
	pSuite->addTest(NumberFormatterTest::suite());
	pSuite->addTest(NumberParserTest::suite());
	pSuite->addTest(DynamicFactoryTest::suite());
	pSuite->addTest(MemoryPoolTest::suite());
	pSuite->addTest(AnyTest::suite());
	pSuite->addTest(DynamicAnyTest::suite());
	pSuite->addTest(FormatTest::suite());
	pSuite->addTest(TuplesTest::suite());
#ifndef POCO_VXWORKS
	pSuite->addTest(NamedTuplesTest::suite());
#endif
	pSuite->addTest(TypeListTest::suite());
	pSuite->addTest(ObjectPoolTest::suite());

	return pSuite;
}
