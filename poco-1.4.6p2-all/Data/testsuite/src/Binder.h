﻿//
// Binder.h
//
// $Id: //poco/1.4/Data/testsuite/src/Binder.h#1 $
//
// Definition of the Binder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Data_Test_Binder_INCLUDED
#define Data_Test_Binder_INCLUDED


#include "Poco/Data/AbstractBinder.h"


namespace Poco
{
namespace Data
{
namespace Test
{


class Binder: public Poco::Data::AbstractBinder
    /// A no-op implementation of AbstractBinder for testing.
{
public:
    Binder();
    /// Creates the Binder.

    ~Binder();
    /// Destroys the Binder.

    void bind(std::size_t pos, const Poco::Int8 &val);
    /// Binds an Int8.

    void bind(std::size_t pos, const Poco::UInt8 &val);
    /// Binds an UInt8.

    void bind(std::size_t pos, const Poco::Int16 &val);
    /// Binds an Int16.

    void bind(std::size_t pos, const Poco::UInt16 &val);
    /// Binds an UInt16.

    void bind(std::size_t pos, const Poco::Int32 &val);
    /// Binds an Int32.

    void bind(std::size_t pos, const Poco::UInt32 &val);
    /// Binds an UInt32.

    void bind(std::size_t pos, const Poco::Int64 &val);
    /// Binds an Int64.

    void bind(std::size_t pos, const Poco::UInt64 &val);
    /// Binds an UInt64.

    void bind(std::size_t pos, const bool &val);
    /// Binds a boolean.

    void bind(std::size_t pos, const float &val);
    /// Binds a float.

    void bind(std::size_t pos, const double &val);
    /// Binds a double.

    void bind(std::size_t pos, const char &val);
    /// Binds a single character.

    void bind(std::size_t pos, const char* const &pVal);
    /// Binds a const char ptr.

    void bind(std::size_t pos, const std::string& val);
    /// Binds a string.

    void bind(std::size_t pos, const Poco::Data::BLOB& val);
    /// Binds a BLOB.

    void bind(std::size_t pos);
    /// Binds a NULL value.

    void reset();
};


}
}
} // namespace Poco::Data::Test


#endif // Data_Test_Binder_INCLUDED
