﻿//
// Extractor.cpp
//
// $Id: //poco/1.4/Data/ODBC/src/Extractor.cpp#1 $
//
// Library: Data/ODBC
// Package: ODBC
// Module:  Extractor
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


#include "Poco/Data/ODBC/Extractor.h"
#include "Poco/Data/ODBC/ODBCColumn.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/BLOB.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Data {
namespace ODBC {


const std::string Extractor::FLD_SIZE_EXCEEDED_FMT = "Specified data size (%z bytes) "
	"exceeds maximum value (%z).\n"
	"Use Session.setProperty(\"maxFieldSize\", value) "
	"to increase the maximum allowed data size\n";


Extractor::Extractor(const StatementHandle& rStmt, 
	Preparation& rPreparation): 
	_rStmt(rStmt), 
	_rPreparation(rPreparation),
	_dataExtraction(rPreparation.getDataExtraction())
{
}


Extractor::~Extractor()
{
}


template<>
bool Extractor::extractBoundImpl<std::string>(std::size_t pos, std::string& val)
{
	std::size_t dataSize = _rPreparation.actualDataSize(pos);
	char*& sp = RefAnyCast<char*>(_rPreparation[pos]);
	poco_check_ptr (sp);
	std::size_t len = std::strlen(sp);
	if (len < dataSize) dataSize = len;
	checkDataSize(dataSize);
	val.assign(sp, dataSize);
	return true;
}


template<>
bool Extractor::extractBoundImpl<Poco::Data::BLOB>(std::size_t pos, Poco::Data::BLOB& val)
{
	std::size_t dataSize = _rPreparation.actualDataSize(pos);
	checkDataSize(dataSize);
	char*& sp = RefAnyCast<char*>(_rPreparation[pos]);
	poco_check_ptr (sp);
	val.assignRaw(sp, dataSize);
	return true;
}


template<>
bool Extractor::extractManualImpl<std::string>(std::size_t pos, std::string& val, SQLSMALLINT cType)
{
	std::size_t maxSize = _rPreparation.getMaxFieldSize();
	std::size_t fetchedSize = 0;
	std::size_t totalSize = 0;

	SQLLEN len;
	Poco::Buffer<char> apChar(CHUNK_SIZE);
	char* pChar = apChar.begin();
	SQLRETURN rc = 0;
	val.clear();

	do
	{
		std::memset(pChar, 0, CHUNK_SIZE);
		len = 0;
		rc = SQLGetData(_rStmt, 
			(SQLUSMALLINT) pos + 1, 
			cType, //C data type
			pChar, //returned value
			CHUNK_SIZE, //buffer length
			&len); //length indicator
		
		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "SQLGetData()");

		if (SQL_NO_TOTAL == len)//unknown length, throw
			throw UnknownDataLengthException("Could not determine returned data length.");

		if (SQL_NO_DATA == rc || SQL_NULL_DATA == len || !len)
			break;

		fetchedSize = len > CHUNK_SIZE ? CHUNK_SIZE : len;
		totalSize += fetchedSize;
		if (totalSize <= maxSize) val.append(pChar, fetchedSize);
		else throw DataException(format(FLD_SIZE_EXCEEDED_FMT, 
				fetchedSize, 
				maxSize));

	}while (true);

	return true;
}


template<>
bool Extractor::extractManualImpl<Poco::Data::BLOB>(std::size_t pos, 
	Poco::Data::BLOB& val, 
	SQLSMALLINT cType)
{
	std::size_t maxSize = _rPreparation.getMaxFieldSize();
	std::size_t fetchedSize = 0;
	std::size_t totalSize = 0;

	SQLLEN len;
	Poco::Buffer<char> apChar(CHUNK_SIZE);
	char* pChar = apChar.begin();
	SQLRETURN rc = 0;
	val.clear();

	do
	{
		std::memset(pChar, 0, CHUNK_SIZE);
		len = 0;
		rc = SQLGetData(_rStmt, 
			(SQLUSMALLINT) pos + 1, 
			cType, //C data type
			pChar, //returned value
			CHUNK_SIZE, //buffer length
			&len); //length indicator
		
		if (SQL_NO_DATA != rc && Utility::isError(rc))
			throw StatementException(_rStmt, "SQLGetData()");

		if (SQL_NO_TOTAL == len)//unknown length, throw
			throw UnknownDataLengthException("Could not determine returned data length.");

		if (SQL_NO_DATA == rc || SQL_NULL_DATA == len || !len)
			break;

		fetchedSize = len > CHUNK_SIZE ? CHUNK_SIZE : len;
		totalSize += fetchedSize;
		if (totalSize <= maxSize) val.appendRaw(pChar, fetchedSize);
		else throw DataException(format(FLD_SIZE_EXCEEDED_FMT, 
				fetchedSize, 
				maxSize));

	}while (true);

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SLONG);
	else
		return extractBoundImpl(pos, val);
	
}


bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SBIGINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, double& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_DOUBLE);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, std::string& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_CHAR);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_BINARY);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_STINYINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_UTINYINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SSHORT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_USHORT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_ULONG);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_SBIGINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, bool& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_BIT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, float& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_FLOAT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, char& val)
{
	if (Preparation::DE_MANUAL == _dataExtraction)
		return extractManualImpl(pos, val, SQL_C_STINYINT);
	else
		return extractBoundImpl(pos, val);
}


bool Extractor::extract(std::size_t pos, Poco::Any& val)
{
	ODBCColumn column(_rStmt, pos);

	switch (column.type())
	{
		case MetaColumn::FDT_INT8:
		{ Poco::Int8 i = 0; extract(pos, i); val = i;	return true; }

		case MetaColumn::FDT_UINT8:
		{ Poco::UInt8 i = 0; extract(pos, i); val = i; return true;	}

		case MetaColumn::FDT_INT16:
		{ Poco::Int16 i = 0; extract(pos, i); val = i; return true;	}

		case MetaColumn::FDT_UINT16:
		{ Poco::UInt16 i = 0; extract(pos, i); val = i; return true; }

		case MetaColumn::FDT_INT32:
		{ Poco::Int32 i = 0; extract(pos, i); val = i; return true;	}

		case MetaColumn::FDT_UINT32:
		{ Poco::UInt32 i = 0; extract(pos, i); val = i; return true; }

		case MetaColumn::FDT_INT64:
		{ Poco::Int64 i = 0; extract(pos, i); val = i; return true;	}

		case MetaColumn::FDT_UINT64:
		{ Poco::UInt64 i = 0; extract(pos, i); val = i; return true; }

		case MetaColumn::FDT_BOOL:
		{ bool b; extract(pos, b); val = b; return true;	}

		case MetaColumn::FDT_FLOAT:
		{ float f; extract(pos, f); val = f; return true;	}

		case MetaColumn::FDT_DOUBLE:
		{ double d; extract(pos, d); val = d; return true;	}

		case MetaColumn::FDT_STRING:
		{ std::string s; extract(pos, s); val = s; return true;	}

		case MetaColumn::FDT_BLOB:
		{ Poco::Data::BLOB b; extract(pos, b); val = b; return true; }

		default: 
			throw DataFormatException("Unsupported data type.");
	}

	return false;
}


void Extractor::checkDataSize(std::size_t size)
{
	std::size_t maxSize = _rPreparation.getMaxFieldSize();
	if (size > maxSize)
	{
		throw DataException(format(FLD_SIZE_EXCEEDED_FMT, 
			size, 
			maxSize));
	}
}


} } } // namespace Poco::Data::ODBC
