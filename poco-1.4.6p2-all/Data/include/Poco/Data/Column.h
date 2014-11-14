﻿//
// Column.h
//
// $Id: //poco/1.4/Data/include/Poco/Data/Column.h#1 $
//
// Library: Data
// Package: DataCore
// Module:  Column
//
// Definition of the Column class.
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


#ifndef Data_Column_INCLUDED
#define Data_Column_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {
namespace Data {


template <class T>
class Column
	/// Column class is column data container.
	/// Data (a pointer to vector of contained values) is assigned to the class 
	/// through either constructor or set() member function.
	/// Construction with null pointer is not allowed.
	/// This class owns the data assigned to it and deletes the storage on destruction.
{
public:
	typedef std::vector<T> DataVec;
	typedef typename DataVec::const_iterator Iterator;
	typedef typename DataVec::size_type Size;

	Column(const MetaColumn& metaColumn, std::vector<T>* pData): _metaColumn(metaColumn), _pData(pData)
		/// Creates the Column.
	{
		poco_check_ptr (_pData);
	}

	Column(const Column& col): _metaColumn(col._metaColumn), _pData(col._pData)
		/// Creates the Column.
	{
	}

	~Column()
		/// Destroys the Column.
	{
	}

	Column& operator = (const Column& col)
		/// Assignment operator.
	{
		Column tmp(col);
		swap(tmp);
		return *this;
	}

	void swap(Column& other)
		/// Swaps the column with another one.
	{
		std::swap(_metaColumn, other._metaColumn);
		std::swap(_pData, other._pData);
	}

	DataVec& data()
		/// Returns reference to contained data.
	{
		return *_pData;
	}

	const T& value(std::size_t row) const
		/// Returns the field value in specified row.
	{
		try
		{
			return _pData->at(row);
		}
		catch (std::out_of_range& ex)
		{ 
			throw RangeException(ex.what()); 
		}
	}

	const T& operator [] (std::size_t row) const
		/// Returns the field value in specified row.
	{
		return value(row);
	}

	Size rowCount() const
		/// Returns number of rows.
	{
		return _pData->size();
	}

	void reset()
		/// Clears and shrinks the storage.
	{
		std::vector<T>().swap(*_pData);
	}

	const std::string& name() const
		/// Returns column name.
	{
		return _metaColumn.name();
	}

	std::size_t length() const
		/// Returns column maximum length.
	{
		return _metaColumn.length();
	}

	std::size_t precision() const
		/// Returns column precision.
		/// Valid for floating point fields only (zero for other data types).
	{
		return _metaColumn.precision();
	}

	std::size_t position() const
		/// Returns column position.
	{
		return _metaColumn.position();
	}

	MetaColumn::ColumnDataType type() const
		/// Returns column type.
	{
		return _metaColumn.type();
	}

	Iterator begin() const
		/// Returns iterator pointing to the beginning of data storage vector.
	{
		return _pData->begin();
	}

	Iterator end() const
		/// Returns iterator pointing to the end of data storage vector.
	{
		return _pData->end();
	}

private:
	Column();

	MetaColumn               _metaColumn;
	Poco::SharedPtr<DataVec> _pData;
};


} } // namespace Poco::Data


#endif // Data_Column_INCLUDED

