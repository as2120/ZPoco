﻿//
// Mutex_VX.cpp
//
// $Id: //poco/1.4/Foundation/src/Mutex_VX.cpp#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Mutex_VX.h"
#include <sysLib.h>


namespace Poco
{


MutexImpl::MutexImpl()
{
    _sem = semMCreate(SEM_INVERSION_SAFE | SEM_Q_PRIORITY);
    if (_sem == 0)
        throw Poco::SystemException("cannot create mutex");
}


MutexImpl::MutexImpl(bool fast)
{
    if (fast)
    {
        _sem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
    }
    else
    {
        _sem = semMCreate(SEM_INVERSION_SAFE | SEM_Q_PRIORITY);
    }
    if (_sem == 0)
        throw Poco::SystemException("cannot create mutex");
}


MutexImpl::~MutexImpl()
{
    semDelete(_sem);
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
    int ticks = milliseconds*sysClkRateGet()/1000;
    return semTake(_sem, ticks) == OK;
}


FastMutexImpl::FastMutexImpl(): MutexImpl(true)
{
}


FastMutexImpl::~FastMutexImpl()
{
}


} // namespace Poco
