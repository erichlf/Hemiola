/*
  MIT License
  Copyright (c) 2021 Erich L Foster
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "HID.h"

#include <cassert>

#include <fcntl.h>
#include <unistd.h>

#include "Exceptions.h"

using namespace hemiola;

hemiola::HID::HID ( const std::string& device )
    : m_HIDId { -1 }
    , m_HIDString { device }
    , m_Opened { false }
{}

hemiola::HID::HID()
    : HID ( "" )
{}

hemiola::HID::~HID()
{
    close();
}

void hemiola::HID::open ( int perms )
{
    assert ( !m_HIDString.empty() );
    assert ( !m_Opened );

    // open input device for reading
    m_HIDId = ::open ( m_HIDString.c_str(), perms );

    if ( m_HIDId == -1 ) {
        throw IoException ( "Error opening input event device '" + m_HIDString + "'", errno );
    }

    m_Opened = true;
}

void hemiola::HID::close()
{
    if ( m_Opened ) {
        ::close ( m_HIDId );
        m_Opened = false;
    }
}
