﻿//
// DTDHandler.h
//
// $Id: //poco/1.4/XML/include/Poco/SAX/DTDHandler.h#1 $
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// SAX DTDHandler Interface.
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


#ifndef SAX_DTDHandler_INCLUDED
#define SAX_DTDHandler_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/XML/XMLString.h"


namespace Poco {
namespace XML {


class XML_API DTDHandler
	/// If a SAX application needs information about notations and unparsed entities, 
	/// then the application implements this interface and registers an instance with the 
	/// SAX parser using the parser's setDTDHandler method. The parser uses the instance 
	/// to report notation and unparsed entity declarations to the application.
	///
	/// Note that this interface includes only those DTD events that the XML recommendation 
	/// requires processors to report: notation and unparsed entity declarations.
	///
	/// The SAX parser may report these events in any order, regardless of the order in 
	/// which the notations and unparsed entities were declared; however, all DTD events 
	/// must be reported after the document handler's startDocument event, and before the first 
	/// startElement event. (If the LexicalHandler is used, these events must also be reported before the endDTD event.)
	/// 
	/// It is up to the application to store the information for future use (perhaps in a hash table or 
	/// object tree). If the application encounters attributes of type "NOTATION", "ENTITY", or "ENTITIES", 
	/// it can use the information that it obtained through this interface to find the entity and/or notation
	/// corresponding with the attribute value.
{
public:
	virtual void notationDecl(const XMLString& name, const XMLString* publicId, const XMLString* systemId) = 0;
		/// Receive notification of a notation declaration event.
		/// 
		/// It is up to the application to record the notation for later reference, 
		/// if necessary; notations may appear as attribute values and in unparsed 
		/// entity declarations, and are sometime used with processing instruction 
		/// target names.
		/// 
		/// At least one of publicId and systemId must be non-null. If a system identifier 
		/// is present, and it is a URL, the SAX parser must resolve it fully before passing 
		/// it to the application through this event.
		/// 
		/// There is no guarantee that the notation declaration will be reported before any 
		/// unparsed entities that use it.
		///
		/// Note that publicId and systemId maybe null, therefore we pass a pointer rather than a reference.
		
	virtual void unparsedEntityDecl(const XMLString& name, const XMLString* publicId, const XMLString& systemId, const XMLString& notationName) = 0;
		/// Receive notification of an unparsed entity declaration event.
		/// 
		/// Note that the notation name corresponds to a notation reported by the 
		/// notationDecl event. It is up to the application to record the entity for 
		/// later reference, if necessary; unparsed entities may appear as attribute values.
		/// 
		/// If the system identifier is a URL, the parser must resolve it fully before 
		/// passing it to the application.
		/// 
		/// Note that publicId maybe null, therefore we pass a pointer rather than a reference.

protected:
	virtual ~DTDHandler();
};


} } // namespace Poco::XML


#endif // SAX_DTDHandler_INCLUDED
