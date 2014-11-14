﻿//
// DataTest.cpp
//
// $Id: //poco/1.4/Data/testsuite/src/DataTest.cpp#1 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "DataTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/Common.h"
#include "Poco/Data/BLOB.h"
#include "Poco/Data/BLOBStream.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/Column.h"
#include "Connector.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/Types.h"
#include "Poco/Exception.h"
#include <cstring>


using namespace Poco::Data;


using Poco::BinaryReader;
using Poco::BinaryWriter;
using Poco::UInt32;
using Poco::Int64;
using Poco::UInt64;
using Poco::InvalidAccessException;
using Poco::RangeException;


DataTest::DataTest(const std::string& name): CppUnit::TestCase(name)
{
	Poco::Data::Test::Connector::addToFactory();
}


DataTest::~DataTest()
{
	Poco::Data::Test::Connector::removeFromFactory();
}


void DataTest::testSession()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	sess << "DROP TABLE IF EXISTS Test", now;
	int count;
	sess << "SELECT COUNT(*) FROM PERSON", into(count), now;
	
	std::string str;
	Statement stmt = (sess << "SELECT * FROM Strings", into(str), limit(50));
	stmt.execute();
}


void DataTest::testFeatures()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	
	sess.setFeature("f1", true);
	assert (sess.getFeature("f1"));
	assert (sess.getFeature("f2"));
	
	try
	{
		sess.setFeature("f2", false);
	}
	catch (NotImplementedException&)
	{
	}
	
	sess.setFeature("f3", false);
	assert (!sess.getFeature("f2"));
	
	try
	{
		sess.setFeature("f3", true);
	}
	catch (NotImplementedException&)
	{
	}
	
	try
	{
		sess.setFeature("f4", false);
	}
	catch (NotSupportedException&)
	{
	}
}


void DataTest::testProperties()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
		
	sess.setProperty("p1", 1);
	Poco::Any v1 = sess.getProperty("p1");
	assert (Poco::AnyCast<int>(v1) == 1);
	Poco::Any v2 = sess.getProperty("p2");
	assert (Poco::AnyCast<int>(v2) == 1);
	
	try
	{
		sess.setProperty("p2", 2);
	}
	catch (NotImplementedException&)
	{
	}
	
	sess.setProperty("p3", 2);
	v1 = sess.getProperty("p2");
	assert (Poco::AnyCast<int>(v1) == 2);
	
	try
	{
		sess.setProperty("p3", 3);
	}
	catch (NotImplementedException&)
	{
	}
	
	try
	{
		sess.setProperty("p4", 4);
	}
	catch (NotSupportedException&)
	{
	}
}


void DataTest::testBLOB()
{
	std::string strDigit = "1234567890";
	std::string strAlpha = "abcdefghijklmnopqrstuvwxyz";
	std::vector<char> vecAlpha(strAlpha.begin(), strAlpha.end());
	std::vector<char> vecDigit(strDigit.begin(), strDigit.end());
	
	Poco::Data::BLOB blobNumStr(strDigit.c_str(), strDigit.size());
	assert (blobNumStr.size() == strDigit.size());
	assert (0 == std::strncmp(strDigit.c_str(), blobNumStr.rawContent(), blobNumStr.size()));
	assert (*blobNumStr.begin() == '1');
	assert (*(blobNumStr.end()-1) == '0');
	Poco::Data::BLOB blobNumVec(vecDigit);
	assert (blobNumVec.size() == vecDigit.size());
	assert (blobNumVec == blobNumStr);
	blobNumVec.swap(blobNumStr);
	assert (blobNumVec.size() == blobNumStr.size());
	assert (blobNumVec == blobNumStr);

	Poco::Data::BLOB blobChrStr(strAlpha.c_str(), strAlpha.size());
	Poco::Data::BLOB blobChrVec(vecAlpha);
	assert (blobChrStr.size() == strAlpha.size());
	assert (0 == std::strncmp(strAlpha.c_str(), blobChrStr.rawContent(), blobChrStr.size()));
	assert (*blobChrStr.begin() == 'a');
	assert (*(blobChrStr.end()-1) == 'z');
	assert (blobChrStr == blobChrVec);

	blobNumStr.swap(blobChrStr);
	assert (blobNumStr != blobChrStr);
	assert (&blobNumStr != &blobChrStr);
	assert (blobNumStr.content() != blobChrStr.content());
	assert (&blobNumStr.content() != &blobChrStr.content());
	assert (blobNumStr == blobChrVec);
}


void DataTest::testBLOBStreams()
{
	Poco::Data::BLOB blob;
	assert (0 == blob.size());

	BLOBOutputStream bos(blob);
	BinaryWriter bw(bos);

	assert (0 == blob.size());
	writeToBLOB(bw);
	assert (blob.size() > 0);

	BLOBInputStream bis(blob);
	BinaryReader br(bis);

	readFromBLOB(br);
}


void DataTest::writeToBLOB(BinaryWriter& writer)
{
	writer << true;
	writer << false;
	writer << 'a';

	writer << (short) -100;
	writer << (unsigned short) 50000;
	writer << -123456;
	writer << (unsigned) 123456;
	writer << (long) -1234567890;
	writer << (unsigned long) 1234567890;
	
	writer << (Int64) -1234567890;
	writer << (UInt64) 1234567890;

	writer << (float) 1.5;
	writer << (double) -1.5;
	
	writer << "foo";
	writer << "";

	writer << std::string("bar");
	writer << std::string();
	
	writer.write7BitEncoded((UInt32) 100);
	writer.write7BitEncoded((UInt32) 1000);
	writer.write7BitEncoded((UInt32) 10000);
	writer.write7BitEncoded((UInt32) 100000);
	writer.write7BitEncoded((UInt32) 1000000);

	writer.write7BitEncoded((UInt64) 100);
	writer.write7BitEncoded((UInt64) 1000);
	writer.write7BitEncoded((UInt64) 10000);
	writer.write7BitEncoded((UInt64) 100000);
	writer.write7BitEncoded((UInt64) 1000000);

	writer.writeRaw("RAW");
}


void DataTest::readFromBLOB(BinaryReader& reader)
{
	bool b = false;
	reader >> b;
	assert (b);
	reader >> b;
	assert (!b);
	
	char c = ' ';
	reader >> c;
	assert (c == 'a');

	short shortv = 0;
	reader >> shortv;
	assert (shortv == -100);

	unsigned short ushortv = 0;
	reader >> ushortv;
	assert (ushortv == 50000);

	int intv = 0;
	reader >> intv;
	assert (intv == -123456);

	unsigned uintv = 0;
	reader >> uintv;
	assert (uintv == 123456);

	long longv = 0;
	reader >> longv;
	assert (longv == -1234567890);

	unsigned long ulongv = 0;
	reader >> ulongv;
	assert (ulongv == 1234567890);

	Int64 int64v = 0;
	reader >> int64v;
	assert (int64v == -1234567890);
	
	UInt64 uint64v = 0;
	reader >> uint64v;
	assert (uint64v == 1234567890);

	float floatv = 0.0;
	reader >> floatv;
	assert (floatv == 1.5);
	
	double doublev = 0.0;
	reader >> doublev;
	assert (doublev == -1.5);

	std::string str;
	reader >> str;
	assert (str == "foo");
	reader >> str;
	assert (str == "");
	
	reader >> str;
	assert (str == "bar");
	reader >> str;
	assert (str == "");

	UInt32 uint32v;
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 100);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 1000);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 10000);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 100000);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 1000000);

	reader.read7BitEncoded(uint64v);
	assert (uint64v == 100);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 1000);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 10000);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 100000);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 1000000);

	reader.readRaw(3, str);
	assert (str == "RAW");
}


void DataTest::testColumn()
{
	MetaColumn mc(0, "mc", MetaColumn::FDT_DOUBLE, 2, 3, true);

	assert (mc.name() == "mc");
	assert (mc.position() == 0);
	assert (mc.length() == 2);
	assert (mc.precision() == 3);
	assert (mc.type() == MetaColumn::FDT_DOUBLE);
	assert (mc.isNullable());

	std::vector<int>* pData = new std::vector<int>;
	pData->push_back(1);
	pData->push_back(2);
	pData->push_back(3);
	pData->push_back(4);
	pData->push_back(5);
	
	Column<int> c(mc, pData);

	assert (c.rowCount() == 5);
	assert (c[0] == 1);
	assert (c[1] == 2);
	assert (c[2] == 3);
	assert (c[3] == 4);
	assert (c[4] == 5);
	assert (c.name() == "mc");
	assert (c.position() == 0);
	assert (c.length() == 2);
	assert (c.precision() == 3);
	assert (c.type() == MetaColumn::FDT_DOUBLE);

	try
	{
		int i; i = c[100];
		fail ("must fail");
	}
	catch (RangeException&) { }

	Column<int> c1 = c;

	assert (c1.rowCount() == 5);
	assert (c1[0] == 1);
	assert (c1[1] == 2);
	assert (c1[2] == 3);
	assert (c1[3] == 4);
	assert (c1[4] == 5);

	Column<int> c2(c1);

	assert (c2.rowCount() == 5);
	assert (c2[0] == 1);
	assert (c2[1] == 2);
	assert (c2[2] == 3);
	assert (c2[3] == 4);
	assert (c2[4] == 5);

	std::vector<int> vi;
	vi.assign(c.begin(), c.end());
	assert (vi.size() == 5);
	assert (vi[0] == 1);
	assert (vi[1] == 2);
	assert (vi[2] == 3);
	assert (vi[3] == 4);
	assert (vi[4] == 5);

	c.reset();
	assert (c.rowCount() == 0);
	assert (c1.rowCount() == 0);
	assert (c2.rowCount() == 0);

}


void DataTest::setUp()
{
}


void DataTest::tearDown()
{
}


CppUnit::Test* DataTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DataTest");

	CppUnit_addTest(pSuite, DataTest, testSession);
	CppUnit_addTest(pSuite, DataTest, testFeatures);
	CppUnit_addTest(pSuite, DataTest, testProperties);
	CppUnit_addTest(pSuite, DataTest, testBLOB);
	CppUnit_addTest(pSuite, DataTest, testBLOBStreams);
	CppUnit_addTest(pSuite, DataTest, testColumn);

	return pSuite;
}
