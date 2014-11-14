﻿//
// DOMWriter.cpp
//
// $Id: //poco/1.4/XML/src/DOMWriter.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOMWriter
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



#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DocumentFragment.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/DOMException.h"
#include "Poco/DOM/DOMSerializer.h"
#include "Poco/SAX/LexicalHandler.h"
#include "Poco/XML/XMLException.h"
#include "Poco/Path.h"
#include "Poco/FileStream.h"


namespace Poco {
namespace XML {


DOMWriter::DOMWriter():
	_pTextEncoding(0),
	_options(0),
	_indent("\t")
{
}


DOMWriter::~DOMWriter()
{
}


void DOMWriter::setEncoding(const std::string& encodingName, Poco::TextEncoding& textEncoding)
{
	_encodingName  = encodingName;
	_pTextEncoding = &textEncoding;
}


void DOMWriter::setOptions(int options)
{
	_options = options;
}


void DOMWriter::setNewLine(const std::string& newLine)
{
	_newLine = newLine;
}


void DOMWriter::setIndent(const std::string& indent)
{
	_indent = indent;
}


void DOMWriter::writeNode(XMLByteOutputStream& ostr, const Node* pNode)
{
	poco_check_ptr (pNode);

	bool isFragment = pNode->nodeType() != Node::DOCUMENT_NODE;

	XMLWriter writer(ostr, _options, _encodingName, _pTextEncoding);
	writer.setNewLine(_newLine);
	writer.setIndent(_indent);
	
	DOMSerializer serializer;
	serializer.setContentHandler(&writer);
	serializer.setDTDHandler(&writer);
	serializer.setProperty(XMLReader::PROPERTY_LEXICAL_HANDLER, static_cast<LexicalHandler*>(&writer));
	if (isFragment) writer.startFragment();
	serializer.serialize(pNode);
	if (isFragment) writer.endFragment();
}


void DOMWriter::writeNode(const std::string& systemId, const Node* pNode)
{
	Poco::FileOutputStream ostr(systemId);
	if (ostr.good())
		writeNode(ostr, pNode);
	else 
		throw Poco::CreateFileException(systemId);
}


} } // namespace Poco::XML

