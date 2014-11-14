﻿//
// unzip.cpp
//
// $Id: //poco/1.4/Zip/samples/unzip/src/unzip.cpp#1 $
//
// This sample demonstrates the Decompress class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Delegate.h"
#include <iostream>
#include <fstream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::Zip::ZipLocalFileHeader;
using Poco::AutoPtr;


class DecompressHandler
{
public:
	DecompressHandler()
	{
	}

	~DecompressHandler()
	{
	}

	void onError(const void*, std::pair<const ZipLocalFileHeader, const std::string>& info)
	{
		Poco::Util::Application::instance().logger().error("ERR: " + info.second);
	}

	void onOk(const void*, std::pair<const ZipLocalFileHeader, const Poco::Path>& info)
	{
		Poco::Util::Application::instance().logger().information("OK: " + info.second.toString(Poco::Path::PATH_UNIX));
	}
};


class UnzipApp: public Application
	/// This sample demonstrates some of the features of the Util::Application class,
	/// such as configuration file handling and command line arguments processing.
	///
	/// Try zip --help (on Unix platforms) or zip /help (elsewhere) for
	/// more information.
{
public:
	UnzipApp(): _helpRequested(false)
	{
	}

protected:	
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		// add your own initialization code here
	}
	
	void uninitialize()
	{
		// add your own uninitialization code here
		Application::uninitialize();
	}
	
	void reinitialize(Application& self)
	{
		Application::reinitialize(self);
		// add your own reinitialization code here
	}
	
	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<UnzipApp>(this, &UnzipApp::handleHelp)));

		options.addOption(
			Option("file", "f", "specifies the input zip file")
				.required(true)
				.repeatable(false)
				.argument("filename")
				.callback(OptionCallback<UnzipApp>(this, &UnzipApp::handleFile)));
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleFile(const std::string& name, const std::string& value)
	{
		_zipFile = value;
	}
	
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS outdir");
		helpFormatter.setHeader("A application that demonstrates usage of Poco::Zip::Decompress class.");
		helpFormatter.format(std::cout);
	}
	

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			Poco::Path outputDir;
			if (!args.empty())
				outputDir.parseDirectory(args[0]);

			std::ifstream in(_zipFile.c_str(), std::ios::binary);
			Poco::Zip::Decompress c(in, outputDir);
			DecompressHandler handler;
			c.EError += Poco::Delegate<DecompressHandler, std::pair<const ZipLocalFileHeader, const std::string> >(&handler, &DecompressHandler::onError);
			c.EOk +=Poco::Delegate<DecompressHandler, std::pair<const ZipLocalFileHeader, const Poco::Path> >(&handler, &DecompressHandler::onOk);
			c.decompressAllFiles();
			c.EError -= Poco::Delegate<DecompressHandler, std::pair<const ZipLocalFileHeader, const std::string> >(&handler, &DecompressHandler::onError);
			c.EOk -=Poco::Delegate<DecompressHandler, std::pair<const ZipLocalFileHeader, const Poco::Path> >(&handler, &DecompressHandler::onOk);
		}
		return Application::EXIT_OK;
	}
	
private:
	bool _helpRequested;
	std::string _zipFile;
};


POCO_APP_MAIN(UnzipApp)
