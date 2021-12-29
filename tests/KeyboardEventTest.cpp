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
#include <gtest/gtest.h>

#include "Exceptions.h"
#include "FakeInputHID.h"
#include "KeyboardEvents.h"
#include "Utils.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <linux/input.h>

TEST ( KeyboardEventTest, KeyPressTest )
{
    auto device = std::make_shared<hemiola::FakeInputHID>();
    device->open();

    std::vector<input_event> data;
    std::vector<unsigned short> expectedPassData;
    std::vector<std::variant<wchar_t, unsigned short>> expectedData;
    std::variant<wchar_t, unsigned short> empty;
    auto addData
        = [&data, &expectedData, &expectedPassData, &empty] ( const input_event& event,
                                           const std::variant<wchar_t, unsigned short>& letter ) {
              data.push_back ( event );
              expectedPassData.push_back ( event.code );
              if ( letter != empty ) {
                  expectedData.push_back ( letter );
              }
          };

    // invalid scanCode
    addData ( input_event { .type = EV_KEY, .code = 129, .value = EV_MAKE },
              static_cast<unsigned short> ( 129 ) );
    addData ( input_event { .type = EV_KEY, .code = KEY_5, .value = EV_MAKE }, wchar_t ( '5' ) );
    addData ( input_event { .type = EV_KEY, .code = KEY_M, .value = EV_MAKE }, wchar_t ( 'm' ) );
    addData ( input_event { .type = EV_KEY, .code = KEY_BACKSLASH, .value = EV_MAKE },
              wchar_t ( '\\' ) );

    // ctrl + c
    addData ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK }, empty );
    addData ( input_event { .type = EV_KEY, .code = KEY_RIGHTCTRL, .value = EV_MAKE },
              static_cast<unsigned short> ( KEY_RIGHTCTRL ) );
    addData ( input_event { .type = EV_KEY, .code = KEY_C, .value = EV_MAKE }, wchar_t ( 'c' ) );

    addData ( input_event { .type = EV_KEY, .code = KEY_F12, .value = EV_MAKE },
              static_cast<unsigned short> ( KEY_F12 ) );

    // shift + z
    addData ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK }, empty );
    addData ( input_event { .type = EV_KEY, .code = KEY_Z, .value = EV_MAKE }, wchar_t ( 'Z' ) );
    addData ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_MAKE }, empty );

    // shift was pressed but then released, so we shouldn't see a shift key in this case
    addData ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK }, empty );
    addData ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_MAKE }, empty );
    addData ( input_event { .type = EV_KEY, .code = KEY_EQUAL, .value = EV_MAKE },
              wchar_t ( '=' ) );

    device->setData ( data );
    // receivedData will be the reverse of this data, because I didn't use a queue
    std::reverse ( expectedData.begin(), expectedData.end() );
    std::reverse ( expectedPassData.begin(), expectedPassData.end() );

    auto keyTable = std::make_shared<hemiola::KeyTable>();

    hemiola::KeyboardEvents keys ( keyTable, device );
    // all data should be passed to output initially
    std::vector<unsigned short> passData;
    auto passThrough = [&passData] ( hemiola::KeyState keyState ) {
        passData.push_back ( keyState.event.code );
    };

    // data that will be captured by keys
    std::vector<std::variant<wchar_t, unsigned short>> receivedData;
    auto onEvent = [&receivedData] ( std::variant<wchar_t, unsigned short> event ) {
        receivedData.push_back ( event );
    };

    // the exception that will be thrown by keys
    std::exception_ptr e;
    auto onError = [&e] ( std::exception_ptr exc ) { e = exc; };

    // normally this would be run in its own thread
    keys.capture ( passThrough, onEvent, onError );

    if ( e != nullptr ) {
        try {
            std::rethrow_exception ( e );
        } catch ( const hemiola::IoException& ex ) {
            EXPECT_EQ ( ex.what(), std::string ( "No more data to read." ) );
            EXPECT_EQ ( ex.code(), 42 );
        } catch ( ... ) {
            FAIL() << "Expected an IoException but got something else.";
        }
    } else {
        FAIL() << "Expected an Exception but didn't get one.";
    }

    EXPECT_EQ ( passData.size(), expectedPassData.size() );

    for ( int i = 0; i < expectedPassData.size(); ++i ) {
        EXPECT_EQ ( passData.at ( i ), expectedPassData.at ( i ) );
    }

    EXPECT_EQ ( receivedData.size(), expectedData.size() );
    for ( int i = 0; i < expectedData.size(); ++i ) {
        EXPECT_EQ ( receivedData.at ( i ), expectedData.at ( i ) );
    }
}
