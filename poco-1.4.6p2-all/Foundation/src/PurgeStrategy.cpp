﻿//
// PurgeStrategy.cpp
//
// $Id: //poco/1.4/Foundation/src/PurgeStrategy.cpp#1 $
//
// Library: Foundation
// Package: Logging
// Module:  FileChannel
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


#include "Poco/PurgeStrategy.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Timestamp.h"


namespace Poco {


//
// PurgeStrategy
//


PurgeStrategy::PurgeStrategy()
{
}


PurgeStrategy::~PurgeStrategy()
{
}


void PurgeStrategy::list(const std::string& path, std::vector<File>& files)
{
	Path p(path);
	p.makeAbsolute();
	Path parent = p.parent();
	std::string baseName = p.getFileName();
	baseName.append(".");

	DirectoryIterator it(parent);
	DirectoryIterator end;
	while (it != end)
	{
		if (it.name().compare(0, baseName.size(), baseName) == 0)
		{
			files.push_back(*it);
		}
		++it;
	}
}


//
// PurgeByAgeStrategy
//


PurgeByAgeStrategy::PurgeByAgeStrategy(const Timespan& age): _age(age)
{
}


PurgeByAgeStrategy::~PurgeByAgeStrategy()
{
}


void PurgeByAgeStrategy::purge(const std::string& path)
{
	std::vector<File> files;
	list(path, files);
	for (std::vector<File>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if (it->getLastModified().isElapsed(_age.totalMicroseconds()))
		{
			it->remove();
		}
	}
}


//
// PurgeByCountStrategy
//


PurgeByCountStrategy::PurgeByCountStrategy(int count): _count(count)
{
	poco_assert(count > 0);
}


PurgeByCountStrategy::~PurgeByCountStrategy()
{
}


void PurgeByCountStrategy::purge(const std::string& path)
{
	std::vector<File> files;
	list(path, files);
	while (files.size() > _count)
	{
		std::vector<File>::iterator it = files.begin();
		std::vector<File>::iterator purgeIt = it;
		Timestamp purgeTS = purgeIt->getLastModified();
		++it;
		while (it != files.end())
		{
			Timestamp md(it->getLastModified());
			if (md <= purgeTS)
			{
				purgeTS = md;
				purgeIt = it;
			}
			++it;
		}
		purgeIt->remove();
		files.erase(purgeIt);
	}
}


} // namespace Poco
