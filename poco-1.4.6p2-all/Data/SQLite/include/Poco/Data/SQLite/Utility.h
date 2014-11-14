﻿//
// Utility.h
//
// $Id: //poco/1.4/Data/SQLite/include/Poco/Data/SQLite/Utility.h#1 $
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Utility
//
// Definition of Utility.
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


#ifndef SQLite_Utility_INCLUDED
#define SQLite_Utility_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Mutex.h"
#include <map>


struct sqlite3;
struct sqlite3_stmt;


namespace Poco {
namespace Data {
namespace SQLite {


class SQLite_API Utility
	/// Various utility functions for SQLite, mostly return code handling
{
public:
	typedef std::map<std::string, MetaColumn::ColumnDataType> TypeMap;
		/// Maps SQLite column declared types to Poco::Data types through
		/// static TypeMap member.
		/// Note: SQLite is type-agnostic and it is the end-user responsibility
		/// to ensure that column declared data type corresponds to the type of 
		/// data actually held in the database.
		/// Column types are case-insensitive.

	static std::string lastError(sqlite3* pDb);
		/// Retreives the last error code from sqlite and converts it to a string

	static void throwException(int rc, const std::string& addErrMsg = std::string());
		/// Throws for an error code the appropriate exception

	static MetaColumn::ColumnDataType getColumnType(sqlite3_stmt* pStmt, std::size_t pos);
		/// Returns column data type.

private:
	static void initTypeMap();

	static TypeMap _types;
	static Poco::FastMutex _mutex;
};


} } } // namespace Poco::Data::SQLite


#endif
