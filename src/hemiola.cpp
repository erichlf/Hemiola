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
#include "Exceptions.h"
#include "InputHID.h"
#include "KeyboardEvents.h"
#include "KeyTable.h"
#include "OutputHID.h"

#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    using namespace hemiola;

    // TODO: Implement a config to get out devices and key maps
    auto input = std::make_shared<InputHID>();
    auto output = std::make_shared<OutputHID>();
    auto keys = std::make_shared<KeyTable>();

    // open devices so they can be used
    input->open();
    output->open();

    KeyboardEvents eventHandler ( keys, input );

    // the exception that will be thrown by keys
    std::exception_ptr e;
    auto onError = [&e] ( std::exception_ptr exc ) { e = exc; };

    auto passThrough = [&keys, &output, &onError] ( KeyReport report ) {
        try {
            output->write ( std::vector<uint8_t> {
                report.modifiers,
                0x00,
                report.keys[0],
                report.keys[1],
                report.keys[2],
                report.keys[3],
                report.keys[4],
                report.keys[5]
                } );
            output->write (
                std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        } catch ( ... ) {
            onError ( std::current_exception() );
        }
    };

    eventHandler.capture ( passThrough, onError );
}
