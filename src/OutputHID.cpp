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
#include "OutputHID.h"

#include "Exceptions.h"
#include "KeyboardEvents.h"
#include "Logger.h"

#include <fcntl.h>
#include <fmt/ranges.h>
#include <linux/input.h>
#include <unistd.h>

#include <cassert>
#include <vector>

using namespace hemiola;

hemiola::OutputHID::OutputHID ( const std::string& device )
    : HID ( device )
{}

hemiola::OutputHID::~OutputHID()
{
    // make sure all key presses are released
    write ( KeyReport {} );
}

void hemiola::OutputHID::open()
{
    HID::open ( O_WRONLY | O_SYNC );
}

void hemiola::OutputHID::write ( const KeyReport& report ) const
{
    assert ( m_Opened );

    LOG ( DEBUG, "Sending Report: {}, ({})", report.modifiers, fmt::join ( report.keys, ", " ) );
    const auto data = std::vector<uint8_t> { report.modifiers, 0x00,
                                             report.keys [0],  report.keys [1],
                                             report.keys [2],  report.keys [3],
                                             report.keys [4],  report.keys [5] };

    if ( ::write ( m_HIDId, data.data(), sizeof ( uint8_t ) * data.size() ) <= 0 ) {
        throw IoException ( "Unable to write to output device", errno );
    }
}
