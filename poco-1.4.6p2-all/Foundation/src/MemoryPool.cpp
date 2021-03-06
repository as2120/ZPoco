﻿//
// MemoryPool.cpp
//
// $Id: //poco/1.4/Foundation/src/MemoryPool.cpp#1 $
//
// Library: Foundation
// Package: Core
// Module:  MemoryPool
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/MemoryPool.h"
#include "Poco/Exception.h"


namespace Poco
{


MemoryPool::MemoryPool(std::size_t blockSize, int preAlloc, int maxAlloc):
    _blockSize(blockSize),
    _maxAlloc(maxAlloc),
    _allocated(preAlloc)
{
    poco_assert (maxAlloc == 0 || maxAlloc >= preAlloc);
    poco_assert (preAlloc >= 0 && maxAlloc >= 0);

    int r = BLOCK_RESERVE;
    if (preAlloc > r)
        r = preAlloc;
    if (maxAlloc > 0 && maxAlloc < r)
        r = maxAlloc;
    _blocks.reserve(r);
    for (int i = 0; i < preAlloc; ++i)
    {
        _blocks.push_back(new char[_blockSize]);
    }
}


MemoryPool::~MemoryPool()
{
    for (BlockVec::iterator it = _blocks.begin(); it != _blocks.end(); ++it)
    {
        delete [] *it;
    }
}


void* MemoryPool::get()
{
    FastMutex::ScopedLock lock(_mutex);

    if (_blocks.empty())
    {
        if (_maxAlloc == 0 || _allocated < _maxAlloc)
        {
            ++_allocated;
            return new char[_blockSize];
        }
        else throw OutOfMemoryException("MemoryPool exhausted");
    }
    else
    {
        char* ptr = _blocks.back();
        _blocks.pop_back();
        return ptr;
    }
}


void MemoryPool::release(void* ptr)
{
    FastMutex::ScopedLock lock(_mutex);

    _blocks.push_back(reinterpret_cast<char*>(ptr));
}


} // namespace Poco
