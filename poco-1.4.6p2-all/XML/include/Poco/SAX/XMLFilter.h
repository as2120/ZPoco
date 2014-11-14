﻿//
// XMLFilter.h
//
// $Id: //poco/1.4/XML/include/Poco/SAX/XMLFilter.h#1 $
//
// Library: XML
// Package: SAX
// Module:  SAXFilters
//
// SAX2 XMLFilter Interface.
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


#ifndef SAX_XMLFilter_INCLUDED
#define SAX_XMLFilter_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/SAX/XMLReader.h"


namespace Poco {
namespace XML {


class XML_API XMLFilter: public XMLReader
	/// Interface for an XML filter.
	///
	/// An XML filter is like an XML reader, except that it obtains its events from another XML reader 
	/// rather than a primary source like an XML document or database. Filters can modify a stream of 
	/// events as they pass on to the final application.
	/// 
	/// The XMLFilterImpl helper class provides a convenient base for creating SAX2 filters, by passing on 
	/// all EntityResolver, DTDHandler, ContentHandler and ErrorHandler events automatically.
{
public:
	virtual XMLReader* getParent() const = 0;
		/// Set the parent reader.
		/// 
		/// This method allows the application to link the filter to a parent reader (which may be another 
		/// filter). The argument may not be null.
		
	virtual void setParent(XMLReader* pParent) = 0;
		/// Get the parent reader.
		/// 
		/// This method allows the application to query the parent reader (which may be another filter). 
		/// It is generally a bad idea to perform any operations on the parent reader directly: they should 
		/// all pass through this filter.
	
protected:
	virtual ~XMLFilter();
};


} } // namespace Poco::XML


#endif // SAX_XMLFilter_INCLUDED
