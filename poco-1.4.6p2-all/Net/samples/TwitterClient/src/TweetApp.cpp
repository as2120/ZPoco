﻿//
// TwitterApp.cpp
//
// $Id: //poco/1.4/Net/samples/TwitterClient/src/TweetApp.cpp#1 $
//
// A very simple command-line Twitter client.
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


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Twitter.h"
#include <iostream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;


class TweetApp: public Application
	/// A very simple Twitter command-line client.
{
public:
	TweetApp():
		_status(Application::EXIT_OK)
	{
	}
	
protected:	
	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<TweetApp>(this, &TweetApp::handleHelp)));

		options.addOption(
			Option("username", "u", "specify the Twitter user/account name")
				.required(true)
				.repeatable(false)
				.argument("account")
				.callback(OptionCallback<TweetApp>(this, &TweetApp::handleUsername)));
				
		options.addOption(
			Option("password", "p", "specify the Twitter password")
				.required(true)
				.repeatable(false)
				.argument("password")
				.callback(OptionCallback<TweetApp>(this, &TweetApp::handlePassword)));

		options.addOption(
			Option("message", "m", "specify the status message")
				.required(false)
				.repeatable(true)
				.argument("message")
				.callback(OptionCallback<TweetApp>(this, &TweetApp::handleMessage)));
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		displayHelp();
		stopOptionsProcessing();
	}
	
	void handleUsername(const std::string& name, const std::string& value)
	{
		_username = value;
	}
	
	void handlePassword(const std::string& name, const std::string& value)
	{
		_password = value;
	}
	
	void handleMessage(const std::string& name, const std::string& value)
	{
		try
		{
			_twitter.login(_username, _password);
			Poco::Int64 statusId = _twitter.update(value);
			std::cout << statusId << std::endl;
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << exc.displayText() << std::endl;
			_status = Application::EXIT_SOFTWARE;
		}
	}
		
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A simple Twitter command line client for posting status updates.");
		helpFormatter.format(std::cout);
	}
	
	int main(const std::vector<std::string>& args)
	{
		return _status;
	}

private:
	std::string _username;
	std::string _password;
	Twitter _twitter;
	int _status;
};


POCO_APP_MAIN(TweetApp)
