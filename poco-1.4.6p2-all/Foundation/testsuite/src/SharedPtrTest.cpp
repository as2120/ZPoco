﻿//
// SharedPtrTest.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/SharedPtrTest.cpp#2 $
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


#include "SharedPtrTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SharedPtr.h"
#include "Poco/Exception.h"


using Poco::SharedPtr;
using Poco::NullPointerException;


namespace
{
	class TestObject
	{
	public:
		TestObject(const std::string& data): _data(data)
		{
			++_count;
		}
		
		virtual ~TestObject()
		{
			--_count;
		}
		
		const std::string& data()
		{
			return _data;
		}
		
		static int count()
		{
			return _count;
		}
		
	private:
		std::string _data;
		static int _count;
	};
	
	int TestObject::_count = 0;

	class DerivedObject: public TestObject
	{
	public:
		DerivedObject(const std::string& s, int i): TestObject(s), _number(i)
		{
		}

		int number() const
		{
			return _number;
		}
		
	private:
		int _number;
	};
}


SharedPtrTest::SharedPtrTest(const std::string& name): CppUnit::TestCase(name)
{
}


SharedPtrTest::~SharedPtrTest()
{
}


void SharedPtrTest::testSharedPtr()
{
	SharedPtr<TestObject> ptr1;
	assertNull(ptr1.get());
	TestObject* pTO1 = new TestObject("one");
	TestObject* pTO2 = new TestObject("two");
	if (pTO2 < pTO1)
	{
		TestObject* pTmp = pTO1;
		pTO1 = pTO2;
		pTO2 = pTmp;
	}
	assert (pTO1 < pTO2);
	ptr1 = pTO1;
	assert (ptr1.referenceCount() == 1);
	SharedPtr<TestObject> ptr2 = pTO2;
	SharedPtr<TestObject> ptr3 = ptr1;
	assert (ptr1.referenceCount() == 2);
	SharedPtr<TestObject> ptr4;
	assert (ptr1.get() == pTO1);
	assert (ptr1 == pTO1);
	assert (ptr2.get() == pTO2);
	assert (ptr2 == pTO2);
	assert (ptr3.get() == pTO1);
	assert (ptr3 == pTO1);
	
	assert (ptr1 == pTO1);
	assert (ptr1 != pTO2);
	assert (ptr1 < pTO2);
	assert (ptr1 <= pTO2);
	assert (ptr2 > pTO1);
	assert (ptr2 >= pTO1);
	
	assert (ptr1 == ptr3);
	assert (ptr1 != ptr2);
	assert (ptr1 < ptr2);
	assert (ptr1 <= ptr2);
	assert (ptr2 > ptr1);
	assert (ptr2 >= ptr1);
	
	ptr1.swap(ptr2);
	assert (ptr2 < ptr1);
	ptr2.swap(ptr1);

	assert ((ptr1->data() == "one" && ptr2->data() == "two") || (ptr1->data() == "two" && ptr2->data() == "one"));
	
	try
	{
		assert (ptr4->data() == "four");
		fail ("must throw NullPointerException");
	}
	catch (NullPointerException&)
	{
	}
	
	assert (!(ptr4 == ptr1));
	assert (!(ptr4 == ptr2));
	assert (ptr4 != ptr1);
	assert (ptr4 != ptr2);
	
	ptr4 = ptr2;
	assert (ptr4 == ptr2);
	assert (!(ptr4 != ptr2));
	
	assert (TestObject::count() == 2);
	ptr1 = 0;
	ptr2 = 0;
	ptr3 = 0;
	ptr4 = 0;
	assert (TestObject::count() == 0);
	
	{
		SharedPtr<TestObject> ptr = new TestObject("");
		assert (TestObject::count() == 1);
	}
	assert (TestObject::count() == 0);
}


void SharedPtrTest::testImplicitCast()
{
	{
		// null assign test
		SharedPtr<DerivedObject> ptr2;
		assertNull(ptr2.get());
		SharedPtr<TestObject> ptr1 = ptr2;
	}
	{
		SharedPtr<DerivedObject> ptr2(new DerivedObject("test", 666));
		assert (TestObject::count() == 1);
		SharedPtr<TestObject> ptr1 = ptr2;
		assert (TestObject::count() == 1);
	}
	assert (TestObject::count() == 0);
	SharedPtr<TestObject> ptr1 = new DerivedObject("test", 666);
	assert (TestObject::count() == 1);
	ptr1 = 0;
	assert (TestObject::count() == 0);
}


void SharedPtrTest::testExplicitCast()
{
	SharedPtr<TestObject> ptr1 = new DerivedObject("test", 666);
	SharedPtr<DerivedObject> ptr2 = ptr1.cast<DerivedObject>();
	assert (ptr2.get() != 0);

	// cast the other way round must fail
	ptr1 = new TestObject("test");
	assert (TestObject::count() == 2);
	ptr2 = ptr1.cast<DerivedObject>();
	assert (TestObject::count() == 1);
	assert (ptr2.get() == 0);
}


void SharedPtrTest::setUp()
{
}


void SharedPtrTest::tearDown()
{
}


CppUnit::Test* SharedPtrTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SharedPtrTest");

	CppUnit_addTest(pSuite, SharedPtrTest, testSharedPtr);
	CppUnit_addTest(pSuite, SharedPtrTest, testImplicitCast);
	CppUnit_addTest(pSuite, SharedPtrTest, testExplicitCast);

	return pSuite;
}