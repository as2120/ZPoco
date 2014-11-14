﻿//
// DTDMap.cpp
//
// $Id: //poco/1.4/XML/src/DTDMap.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOM
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


#include "Poco/DOM/DTDMap.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/DOMException.h"


namespace Poco {
namespace XML {


DTDMap::DTDMap(const DocumentType* pDocumentType, unsigned short type):
	_pDocumentType(pDocumentType),
	_type(type)
{
	poco_check_ptr (pDocumentType->ownerDocument());
}


DTDMap::~DTDMap()
{
}


Node* DTDMap::getNamedItem(const XMLString& name) const
{
	Node* pCur = _pDocumentType->firstChild();
	while (pCur)
	{
		if (pCur->nodeType() == _type && pCur->nodeName() == name)
			return pCur;
		pCur = pCur->nextSibling();
	}
	return pCur;
}


Node* DTDMap::setNamedItem(Node* arg)
{
	throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
}


Node* DTDMap::removeNamedItem(const XMLString& name)
{
	throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
}


Node* DTDMap::item(unsigned long index) const
{
	unsigned long n = 0;
	Node* pCur = _pDocumentType->firstChild();
	while (pCur)
	{
		if (pCur->nodeType() == _type)
		{
			if (n == index) return pCur;
			++n;
		}
		pCur = pCur->nextSibling();
	}
	return pCur;
}


unsigned long DTDMap::length() const
{
	unsigned long n = 0;
	Node* pCur = _pDocumentType->firstChild();
	while (pCur)
	{
		if (pCur->nodeType() == _type) ++n;
		pCur = pCur->nextSibling();
	}
	return n;
}


Node* DTDMap::getNamedItemNS(const XMLString& namespaceURI, const XMLString& localName) const
{
	return 0;
}


Node* DTDMap::setNamedItemNS(Node* arg)
{
	throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
}


Node* DTDMap::removeNamedItemNS(const XMLString& namespaceURI, const XMLString& localName)
{
	throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
}


void DTDMap::autoRelease()
{
	_pDocumentType->ownerDocument()->autoReleasePool().add(this);
}


} } // namespace Poco::XML
