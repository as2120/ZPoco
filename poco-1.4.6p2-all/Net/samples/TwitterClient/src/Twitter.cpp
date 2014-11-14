﻿//
// Twitter.cpp
//
// $Id: //poco/1.4/Net/samples/TwitterClient/src/Twitter.cpp#1 $
//
// A C++ implementation of a Twitter client based on the POCO Net library.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
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


#include "Twitter.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/URI.h"
#include "Poco/NumberParser.h"


const std::string Twitter::TWITTER_URI("http://twitter.com/statuses/");


Twitter::Twitter():
	_uri(TWITTER_URI)
{
}

	
Twitter::Twitter(const std::string& twitterURI):
	_uri(twitterURI)
{
}

	
Twitter::~Twitter()
{
}


void Twitter::login(const std::string& username, const std::string& password)
{
	_username = username;
	_password = password;
}

	
Poco::Int64 Twitter::update(const std::string& status)
{
	Poco::Net::HTMLForm form;
	form.set("status", status);
	Poco::AutoPtr<Poco::XML::Document> pResult = invoke(Poco::Net::HTTPRequest::HTTP_POST, "update", form);
	Poco::AutoPtr<Poco::XML::NodeList> pList = pResult->getElementsByTagName("id");
	if (pList->length() > 0)
	{
		return Poco::NumberParser::parse64(pList->item(0)->innerText());
	}
	else return 0;
}


Poco::AutoPtr<Poco::XML::Document> Twitter::invoke(const std::string& httpMethod, const std::string& twitterMethod, Poco::Net::HTMLForm& form)
{
	// Create the request URI.
	// We use the XML version of the Twitter API.
	Poco::URI uri(_uri + twitterMethod + ".xml");
	std::string path(uri.getPath());
	
	Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
	Poco::Net::HTTPRequest req(httpMethod, path, Poco::Net::HTTPMessage::HTTP_1_1);
	
	// Add username and password (HTTP basic authentication) to the request.
	Poco::Net::HTTPBasicCredentials cred(_username, _password);
	cred.authenticate(req);
	
	// Send the request.
	form.prepareSubmit(req);
	std::ostream& ostr = session.sendRequest(req);
	form.write(ostr);
	
	// Receive the response.
	Poco::Net::HTTPResponse res;
	std::istream& rs = session.receiveResponse(res);
	
	// Create a DOM document from the response.
	Poco::XML::DOMParser parser;
	parser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
	Poco::XML::InputSource source(rs);
	Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parse(&source);
	
	// If everything went fine, return the XML document.
	// Otherwise look for an error message in the XML document.
	if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
	{
		return pDoc;
	}
	else
	{
		std::string error(res.getReason());
		Poco::AutoPtr<Poco::XML::NodeList> pList = pDoc->getElementsByTagName("error");
		if (pList->length() > 0)
		{
			error += ": ";
			error += pList->item(0)->innerText();
		}
		throw Poco::ApplicationException("Twitter Error", error);
	}
}
