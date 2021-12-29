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
#include "FakeInputHID.h"

#include "Utils.h"

#include <linux/input.h>

using namespace hemiola;

hemiola::FakeInputHID::FakeInputHID()
    : InputHID()
{}

void hemiola::FakeInputHID::open()
{
    m_Opened = true;
}

void hemiola::FakeInputHID::close()
{
    m_Opened = false;
}

void hemiola::FakeInputHID::read ( input_event& event )
{
    if ( m_Data.size() > 0 ) {
        event = m_Data.back();
        m_Data.pop_back();
    } else {
        throw IoException ( "No more data to read.", 42 );
    }
}

void hemiola::FakeInputHID::setData ( const std::vector<input_event>& events )
{
    m_Data = events;
}
