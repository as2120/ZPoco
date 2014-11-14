﻿//
// Utility.cpp
//
// $Id: //poco/1.4/Data/SQLite/src/Utility.cpp#3 $
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Utility
//
// Implementation of Utility
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


#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#if defined(POCO_UNBUNDLED)
#include <sqlite3.h>
#else
#include "sqlite3.h"
#endif


namespace Poco {
namespace Data {
namespace SQLite {


Utility::TypeMap Utility::_types;
Poco::FastMutex Utility::_mutex;


void Utility::initTypeMap()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_types.empty())
	{
		_types.insert(TypeMap::value_type("", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("BOOL", MetaColumn::FDT_BOOL));
		_types.insert(TypeMap::value_type("BOOLEAN", MetaColumn::FDT_BOOL));
		_types.insert(TypeMap::value_type("BIT", MetaColumn::FDT_BOOL));
		_types.insert(TypeMap::value_type("UINT8", MetaColumn::FDT_UINT8));
		_types.insert(TypeMap::value_type("UTINY", MetaColumn::FDT_UINT8));
		_types.insert(TypeMap::value_type("UINTEGER8", MetaColumn::FDT_UINT8));
		_types.insert(TypeMap::value_type("INT8", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("TINY", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("INTEGER8", MetaColumn::FDT_INT8));
		_types.insert(TypeMap::value_type("UINT16", MetaColumn::FDT_UINT16));
		_types.insert(TypeMap::value_type("USHORT", MetaColumn::FDT_UINT16));
		_types.insert(TypeMap::value_type("UINTEGER16", MetaColumn::FDT_UINT16));
		_types.insert(TypeMap::value_type("INT16", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("SHORT", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("INTEGER16", MetaColumn::FDT_INT16));
		_types.insert(TypeMap::value_type("UINT", MetaColumn::FDT_UINT32));
		_types.insert(TypeMap::value_type("UINT32", MetaColumn::FDT_UINT32));
		_types.insert(TypeMap::value_type("UINTEGER32", MetaColumn::FDT_UINT32));
		_types.insert(TypeMap::value_type("INT", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("INT32", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("INTEGER", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("INTEGER32", MetaColumn::FDT_INT32));
		_types.insert(TypeMap::value_type("UINT64", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("ULONG", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("UINTEGER64", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("INT64", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("LONG", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("INTEGER64", MetaColumn::FDT_INT64));
		_types.insert(TypeMap::value_type("COUNTER", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("AUTOINCREMENT", MetaColumn::FDT_UINT64));
		_types.insert(TypeMap::value_type("REAL", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("FLOA", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("FLOAT", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("DOUB", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("DOUBLE", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("DECIMAL", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("NUMERIC", MetaColumn::FDT_DOUBLE));
		_types.insert(TypeMap::value_type("CHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("CLOB", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("TEXT", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("VARCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NCLOB", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NTEXT", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("NVARCHAR", MetaColumn::FDT_STRING));
		_types.insert(TypeMap::value_type("BLOB", MetaColumn::FDT_BLOB));
	}
}


std::string Utility::lastError(sqlite3 *pDB)
{
	return std::string(sqlite3_errmsg(pDB));
}


MetaColumn::ColumnDataType Utility::getColumnType(sqlite3_stmt* pStmt, std::size_t pos)
{
	poco_assert_dbg (pStmt);

	initTypeMap();
	
	const char* pc = sqlite3_column_decltype(pStmt, (int) pos);
	std::string sqliteType = pc ? pc : "";
	Poco::toUpperInPlace(sqliteType);
	sqliteType = sqliteType.substr(0, sqliteType.find_first_of(" ("));

	TypeMap::const_iterator it = _types.find(Poco::trimInPlace(sqliteType));
	if (_types.end() == it)	throw Poco::NotFoundException();

	return it->second;
}


void Utility::throwException(int rc, const std::string& addErrMsg)
{
	switch (rc)
	{
	case SQLITE_OK:
		break;
	case SQLITE_ERROR:
		throw InvalidSQLStatementException(addErrMsg);
	case SQLITE_INTERNAL:
		throw InternalDBErrorException(addErrMsg);
	case SQLITE_PERM:
		throw DBAccessDeniedException(addErrMsg);
	case SQLITE_ABORT:
		throw ExecutionAbortedException(addErrMsg);
	case SQLITE_BUSY:
		throw DBLockedException(addErrMsg);
	case SQLITE_LOCKED:
	case SQLITE_LOCKED_SHAREDCACHE:
		throw TableLockedException(addErrMsg);
	case SQLITE_NOMEM:
		throw NoMemoryException(addErrMsg);
	case SQLITE_READONLY:
		throw ReadOnlyException(addErrMsg);
	case SQLITE_INTERRUPT:
		throw InterruptException(addErrMsg);
	case SQLITE_IOERR:
		throw IOErrorException(addErrMsg);
	case SQLITE_CORRUPT:
		throw CorruptImageException(addErrMsg);
	case SQLITE_NOTFOUND:
		throw TableNotFoundException(addErrMsg);
	case SQLITE_FULL:
		throw DatabaseFullException(addErrMsg);
	case SQLITE_CANTOPEN:
		throw CantOpenDBFileException(addErrMsg);
	case SQLITE_PROTOCOL:
		throw LockProtocolException(addErrMsg);
	case SQLITE_EMPTY:
		throw InternalDBErrorException(addErrMsg);
	case SQLITE_SCHEMA:
		throw SchemaDiffersException(addErrMsg);
	case SQLITE_TOOBIG:
		throw RowTooBigException(addErrMsg);
	case SQLITE_CONSTRAINT:
		throw ConstraintViolationException(addErrMsg);
	case SQLITE_MISMATCH:
		throw DataTypeMismatchException(addErrMsg);
	case SQLITE_MISUSE:
		throw InvalidLibraryUseException(addErrMsg);
	case SQLITE_NOLFS:
		throw OSFeaturesMissingException(addErrMsg);
	case SQLITE_AUTH:
		throw AuthorizationDeniedException(addErrMsg);
	case SQLITE_FORMAT:
		throw CorruptImageException(addErrMsg);
	case SQLITE_NOTADB:
		throw CorruptImageException(addErrMsg);
	case SQLITE_RANGE:
		throw InvalidSQLStatementException(std::string("Bind parameter out of range (Access of invalid position 0? Bind starts with 1!)"), addErrMsg);
	case SQLITE_ROW:
		break; // sqlite_step() has another row ready
	case SQLITE_DONE:
		break; // sqlite_step() has finished executing
	default:
		throw SQLiteException(std::string("Unknown error code: ") + Poco::NumberFormatter::format(rc), addErrMsg);
	}
}


} } } // namespace Poco::Data::SQLite
