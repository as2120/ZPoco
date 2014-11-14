﻿//
// ODBCMySQLTest.cpp
//
// $Id: //poco/1.4/Data/ODBC/testsuite/src/ODBCMySQLTest.cpp#1 $
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


#include "ODBCMySQLTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Data/Common.h"
#include "Poco/Data/BLOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/ODBC/ODBCStatementImpl.h"
#include <sqltypes.h>
#include <iostream>


#define MYSQL_DRIVER_VERSION "5.1"

#ifdef POCO_OS_FAMILY_WINDOWS
	#define MYSQL_ODBC_DRIVER "MySQL ODBC "##MYSQL_DRIVER_VERSION##" Driver"
#else
	#define MYSQL_ODBC_DRIVER "MySQL"
#endif
#define MYSQL_DSN "PocoDataMySQLTest"
#define MYSQL_SERVER "localhost"
#define MYSQL_DB "test"
#define MYSQL_UID "root"
#define MYSQL_PWD ""


using namespace Poco::Data;
using Poco::Data::ODBC::Utility;
using Poco::Data::ODBC::ConnectionException;
using Poco::Data::ODBC::StatementException;
using Poco::Data::ODBC::StatementDiagnostics;
using Poco::format;
using Poco::NotFoundException;


const bool ODBCMySQLTest::bindValues[8] = {true, true, true, false, false, true, false, false};
Poco::SharedPtr<Poco::Data::Session> ODBCMySQLTest::_pSession = 0;
Poco::SharedPtr<SQLExecutor> ODBCMySQLTest::_pExecutor = 0;
std::string ODBCMySQLTest::_dsn = "PocoDataMySQLTest";
std::string ODBCMySQLTest::_dbConnString;
Poco::Data::ODBC::Utility::DriverMap ODBCMySQLTest::_drivers;
Poco::Data::ODBC::Utility::DSNMap ODBCMySQLTest::_dataSources;


ODBCMySQLTest::ODBCMySQLTest(const std::string& name): 
	CppUnit::TestCase(name)
{
	static bool beenHere = false;

	ODBC::Connector::registerConnector();
	if (_drivers.empty() || _dataSources.empty()) 
	{
		Utility::drivers(_drivers);
		Utility::dataSources(_dataSources);
		checkODBCSetup();
	}
	if (!_pSession && !_dbConnString.empty() && !beenHere)
	{
		try
		{
			_pSession = new Session(SessionFactory::instance().create(ODBC::Connector::KEY, _dbConnString));
			if (_pSession && _pSession->isConnected()) 
				std::cout << "*** Connected to " << _dbConnString << std::endl;
		}catch (ConnectionException& ex)
		{
			std::cout << "!!! WARNING: Connection failed. MySQL tests will fail !!!" << std::endl;
			std::cout << ex.toString() << std::endl;
		}

		if (!_pExecutor) _pExecutor = new SQLExecutor("MySQL SQL Executor", _pSession);
	}
	else 
	if (!_pSession && !beenHere) 
		std::cout << "!!! WARNING: No driver or DSN found. MySQL tests will fail !!!" << std::endl;

	beenHere = true;
}


ODBCMySQLTest::~ODBCMySQLTest()
{
	ODBC::Connector::unregisterConnector();
}


void ODBCMySQLTest::testBareboneODBC()
{
	if (!_pSession) fail ("Test not available.");

	std::string tableCreateString = "CREATE TABLE Test "
		"(First VARCHAR(30),"
		"Second VARCHAR(30),"
		"Third VARBINARY(30),"
		"Fourth INTEGER,"
		"Fifth FLOAT,"
		"Sixth DATETIME)";

	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_MANUAL);
	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_IMMEDIATE, SQLExecutor::DE_BOUND);
	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_MANUAL);
	_pExecutor->bareboneODBCTest(_dbConnString, tableCreateString, SQLExecutor::PB_AT_EXEC, SQLExecutor::DE_BOUND);
}


void ODBCMySQLTest::testSimpleAccess()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->simpleAccess();
		i += 2;
	}
}


void ODBCMySQLTest::testComplexType()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->complexType();
		i += 2;
	}
}


void ODBCMySQLTest::testSimpleAccessVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->simpleAccessVector();
		i += 2;
	}
}


void ODBCMySQLTest::testComplexTypeVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->complexTypeVector();
		i += 2;
	}	
}


void ODBCMySQLTest::testInsertVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertVector();
		i += 2;
	}	
}


void ODBCMySQLTest::testInsertEmptyVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateStringsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertEmptyVector();
		i += 2;
	}	
}


void ODBCMySQLTest::testInsertSingleBulk()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertSingleBulk();
		i += 2;
	}	
}


void ODBCMySQLTest::testInsertSingleBulkVec()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->insertSingleBulkVec();
		i += 2;
	}	
}


void ODBCMySQLTest::testLimit()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->limits();
		i += 2;
	}
}


void ODBCMySQLTest::testLimitZero()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->limitZero();
		i += 2;
	}	
}


void ODBCMySQLTest::testLimitOnce()
{
	if (!_pSession) fail ("Test not available.");

	recreateIntsTable();
	_pExecutor->limitOnce();
	
}


void ODBCMySQLTest::testLimitPrepare()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->limitPrepare();
		i += 2;
	}
}



void ODBCMySQLTest::testPrepare()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateIntsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->prepare();
		i += 2;
	}
}


void ODBCMySQLTest::testSetSimple()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->setSimple();
		i += 2;
	}
}


void ODBCMySQLTest::testSetComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->setComplex();
		i += 2;
	}
}


void ODBCMySQLTest::testSetComplexUnique()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->setComplexUnique();
		i += 2;
	}
}

void ODBCMySQLTest::testMultiSetSimple()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->multiSetSimple();
		i += 2;
	}
}


void ODBCMySQLTest::testMultiSetComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->multiSetComplex();
		i += 2;
	}	
}


void ODBCMySQLTest::testMapComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->mapComplex();
		i += 2;
	}
}


void ODBCMySQLTest::testMapComplexUnique()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->mapComplexUnique();
		i += 2;
	}
}


void ODBCMySQLTest::testMultiMapComplex()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->multiMapComplex();
		i += 2;
	}
}


void ODBCMySQLTest::testSelectIntoSingle()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->selectIntoSingle();
		i += 2;
	}
}


void ODBCMySQLTest::testSelectIntoSingleStep()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->selectIntoSingleStep();
		i += 2;
	}	
}


void ODBCMySQLTest::testSelectIntoSingleFail()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->selectIntoSingleFail();
		i += 2;
	}	
}


void ODBCMySQLTest::testLowerLimitOk()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->lowerLimitOk();
		i += 2;
	}	
}


void ODBCMySQLTest::testSingleSelect()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->singleSelect();
		i += 2;
	}	
}


void ODBCMySQLTest::testLowerLimitFail()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->lowerLimitFail();
		i += 2;
	}
}


void ODBCMySQLTest::testCombinedLimits()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->combinedLimits();
		i += 2;
	}
}



void ODBCMySQLTest::testRange()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->ranges();
		i += 2;
	}
}


void ODBCMySQLTest::testCombinedIllegalLimits()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->combinedIllegalLimits();
		i += 2;
	}
}



void ODBCMySQLTest::testIllegalRange()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->illegalRange();
		i += 2;
	}
}


void ODBCMySQLTest::testEmptyDB()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->emptyDB();
		i += 2;
	}
}


void ODBCMySQLTest::testBLOB()
{
	if (!_pSession) fail ("Test not available.");
	
	const std::size_t maxFldSize = 65534;
	_pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize-1));
	recreatePersonBLOBTable();

	try
	{
		_pExecutor->blob(maxFldSize);
		fail ("must fail");
	}
	catch (DataException&) 
	{
		_pSession->setProperty("maxFieldSize", Poco::Any(maxFldSize));
	}

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->blob(maxFldSize);
		i += 2;
	}

	recreatePersonBLOBTable();
	try
	{
		_pExecutor->blob(maxFldSize+1);
		fail ("must fail");
	}
	catch (DataException&) { }
}


void ODBCMySQLTest::testBLOBStmt()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreatePersonBLOBTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->blobStmt();
		i += 2;
	}
}


void ODBCMySQLTest::testBool()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateBoolsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->bools();
		i += 2;
	}
}


void ODBCMySQLTest::testFloat()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateFloatsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->floats();
		i += 2;
	}
}


void ODBCMySQLTest::testDouble()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateFloatsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->doubles();
		i += 2;
	}
}


void ODBCMySQLTest::testTuple()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateTuplesTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->tuples();
		i += 2;
	}
}


void ODBCMySQLTest::testTupleVector()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateTuplesTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->tupleVector();
		i += 2;
	}
}


void ODBCMySQLTest::testInternalExtraction()
{
	if (!_pSession) fail ("Test not available.");

	for (int i = 0; i < 8;)
	{
		recreateVectorsTable();
		_pSession->setFeature("autoBind", bindValues[i]);
		_pSession->setFeature("autoExtract", bindValues[i+1]);
		_pExecutor->internalExtraction();
		i += 2;
	}
}


void ODBCMySQLTest::dropTable(const std::string& tableName)
{
	try
	{
		*_pSession << format("DROP TABLE %s", tableName), now;
	}
	catch (StatementException& ex)
	{
		bool ignoreError = false;
		const StatementDiagnostics::FieldVec& flds = ex.diagnostics().fields();
		StatementDiagnostics::Iterator it = flds.begin();
		for (; it != flds.end(); ++it)
		{
			if (1051 == it->_nativeError)//(table does not exist)
			{
				ignoreError = true;
				break;
			}
		}

		if (!ignoreError) throw;
	}
}


void ODBCMySQLTest::recreatePersonTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Age INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonTable()"); }
}


void ODBCMySQLTest::recreatePersonBLOBTable()
{
	dropTable("Person");
	try { *_pSession << "CREATE TABLE Person (LastName VARCHAR(30), FirstName VARCHAR(30), Address VARCHAR(30), Image BLOB)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreatePersonBLOBTable()"); }
}


void ODBCMySQLTest::recreateIntsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateIntsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateIntsTable()"); }
}


void ODBCMySQLTest::recreateStringsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateStringsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateStringsTable()"); }
}


void ODBCMySQLTest::recreateBoolsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateBoolsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateBoolsTable()"); }
}


void ODBCMySQLTest::recreateFloatsTable()
{
	dropTable("Strings");
	try { *_pSession << "CREATE TABLE Strings (str FLOAT)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateFloatsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateFloatsTable()"); }
}


void ODBCMySQLTest::recreateTuplesTable()
{
	dropTable("Tuples");
	try { *_pSession << "CREATE TABLE Tuples "
		"(i0 INTEGER, i1 INTEGER, i2 INTEGER, i3 INTEGER, i4 INTEGER, i5 INTEGER, i6 INTEGER, "
		"i7 INTEGER, i8 INTEGER, i9 INTEGER, i10 INTEGER, i11 INTEGER, i12 INTEGER, i13 INTEGER,"
		"i14 INTEGER, i15 INTEGER, i16 INTEGER, i17 INTEGER, i18 INTEGER, i19 INTEGER)", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateTuplesTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateTuplesTable()"); }
}


void ODBCMySQLTest::recreateVectorsTable()
{
	dropTable("Vectors");
	try { *_pSession << "CREATE TABLE Vectors (i0 INTEGER, flt0 FLOAT, str0 VARCHAR(30))", now; }
	catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail ("recreateVectorsTable()"); }
	catch(StatementException& se){ std::cout << se.toString() << std::endl; fail ("recreateVectorsTable()"); }
}


void ODBCMySQLTest::checkODBCSetup()
{
	static bool beenHere = false;

	if (!beenHere)
	{
		beenHere = true;
		
		bool driverFound = false;
		bool dsnFound = false;

		Utility::DriverMap::iterator itDrv = _drivers.begin();
		for (; itDrv != _drivers.end(); ++itDrv)
		{
			if (((itDrv->first).find("MySQL") != std::string::npos))
			{
				std::cout << "Driver found: " << itDrv->first 
					<< " (" << itDrv->second << ')' << std::endl;
				driverFound = true;
				break;
			}
		}

		if (!driverFound) 
		{
			std::cout << "MySQL driver NOT found, tests will fail." << std::endl;
			return;
		}
		
		Utility::DSNMap::iterator itDSN = _dataSources.begin();
		for (; itDSN != _dataSources.end(); ++itDSN)
		{
			if (((itDSN->first).find(_dsn) != std::string::npos) &&
				((itDSN->second).find("MySQL") != std::string::npos))
			{
				std::cout << "DSN found: " << itDSN->first 
					<< " (" << itDSN->second << ')' << std::endl;
				dsnFound = true;
				break;
			}
		}

		if (!dsnFound) 
		{
			if (!_pSession && _dbConnString.empty())
			{
				std::cout << "MySQL DSN NOT found, will attempt to connect without it." << std::endl;
				_dbConnString = "DRIVER=" MYSQL_ODBC_DRIVER ";"
					"DATABASE=" MYSQL_DB ";"
					"SERVER=" MYSQL_SERVER ";"
					"UID=" MYSQL_UID ";"
					"PWD=" MYSQL_PWD ";";
			}
			else if (!_dbConnString.empty())
			{
				std::cout << "MySQL tests not available." << std::endl;
				return;
			}
		}
	}

	if (!_pSession)
		format(_dbConnString, "DSN=%s;", _dsn);
}


void ODBCMySQLTest::setUp()
{
}


void ODBCMySQLTest::tearDown()
{
	dropTable("Person");
	dropTable("Strings");
}


CppUnit::Test* ODBCMySQLTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ODBCMySQLTest");

	CppUnit_addTest(pSuite, ODBCMySQLTest, testBareboneODBC);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSimpleAccess);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testComplexType);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSimpleAccessVector);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testComplexTypeVector);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testInsertVector);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testInsertEmptyVector);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testInsertSingleBulk);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testInsertSingleBulkVec);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testLimit);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testLimitOnce);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testLimitPrepare);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testLimitZero);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testPrepare);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSetSimple);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSetComplex);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSetComplexUnique);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testMultiSetSimple);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testMultiSetComplex);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testMapComplex);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testMapComplexUnique);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testMultiMapComplex);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSelectIntoSingle);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSelectIntoSingleStep);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSelectIntoSingleFail);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testLowerLimitOk);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testLowerLimitFail);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testCombinedLimits);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testCombinedIllegalLimits);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testRange);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testIllegalRange);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testSingleSelect);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testEmptyDB);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testBLOB);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testBLOBStmt);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testBool);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testFloat);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testDouble);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testTuple);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testTupleVector);
	CppUnit_addTest(pSuite, ODBCMySQLTest, testInternalExtraction);

	return pSuite;
}
