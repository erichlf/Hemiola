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
#include <exception>
#include <iostream>
#include <queue>
#include <string>

#include <gtest/gtest.h>
#include <linux/input.h>

#include "Exceptions.h"
#include "FakeInputHID.h"
#include "KeyboardEvents.h"
#include "Utils.h"

namespace hemiola
{
    class Test
    {
    public:
        Test()
            : m_ExpectedData {}
            , m_ReceivedData {}
            , m_Data {}
        {}

        Test ( const Test& ) = delete;
        Test ( Test&& ) = delete;
        Test& operator= ( const Test& ) = delete;
        Test& operator= ( Test&& ) = delete;
        ~Test() = default;

        void addData ( input_event event, KeyReport report )
        {
            m_Data.push ( event );
            m_ExpectedData.push ( report );
        }

        void run()
        {
            auto device = std::make_shared<FakeInputHID>();
            device->setData ( m_Data );

            auto keyTable = std::make_shared<KeyTable>();

            KeyboardEvents keys ( keyTable, device );
            // all data should be passed to output initially
            auto onEvent = [this] ( KeyReport report ) { m_ReceivedData.push ( report ); };

            // the exception that will be thrown by keys
            auto onError = [this] ( std::exception_ptr exc ) { m_E = exc; };

            // normally this would be run in its own thread
            keys.capture ( onEvent, onError );
        }

        std::queue<KeyReport> m_ExpectedData;
        std::queue<KeyReport> m_ReceivedData;
        std::exception_ptr m_E;

    private:
        std::queue<input_event> m_Data;
    };
}

TEST ( KeyboardEventTest, KeyPressTest )
{
    using namespace hemiola;

    hemiola::Test test;

    // invalid scanCode
    test.addData ( input_event { .type = EV_KEY, .code = 129, .value = EV_MAKE }, KeyReport {} );

    // no modifiers
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_5, .value = EV_MAKE },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x22, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_M, .value = EV_MAKE },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_BACKSLASH, .value = EV_MAKE },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x31, 0x00, 0x00, 0x00, 0x00, 0x00 } } );

    test.addData (
        input_event { .type = EV_KEY, .code = KEY_F12, .value = EV_MAKE },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 } } );

    test.addData (
        input_event { .type = EV_KEY, .code = KEY_EQUAL, .value = EV_MAKE },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00 } } );

    test.run();

    if ( test.m_E != nullptr ) {
        try {
            std::rethrow_exception ( test.m_E );
        } catch ( const IoException& ex ) {
            EXPECT_EQ ( ex.what(), std::string ( "No more data to read." ) );
            EXPECT_EQ ( ex.code(), 42 );
        } catch ( ... ) {
            FAIL() << "Expected an IoException but got something else.";
        }
    } else {
        FAIL() << "Expected an Exception but didn't get one.";
    }

    EXPECT_EQ ( test.m_ReceivedData.size(), test.m_ExpectedData.size() );

    for ( int i = 0; i < test.m_ExpectedData.size(); ++i ) {
        EXPECT_EQ ( test.m_ReceivedData.front(), test.m_ExpectedData.front() );
        test.m_ReceivedData.pop();
        test.m_ExpectedData.pop();
    }
}

TEST ( KeyboardEventTest, ModifierTest )
{
    using namespace hemiola;

    hemiola::Test test;

    // ctrl + alt + c
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTALT, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData ( input_event { .type = EV_KEY, .code = KEY_RIGHTCTRL, .value = EV_MAKE },
                   KeyReport { .modifiers = 0x04 | 0x10,
                               .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData ( input_event { .type = EV_KEY, .code = KEY_C, .value = EV_MAKE },
                   KeyReport { .modifiers = 0x04 | 0x10,
                               .keys = KeyArray { 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTALT, .value = EV_BREAK },
        KeyReport { .modifiers = 0x10, .keys = KeyArray { 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData ( input_event { .type = EV_KEY, .code = KEY_RIGHTCTRL, .value = EV_BREAK },
                   KeyReport { .modifiers = 0x00 | 0x00,
                               .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    // alt + a + b + c + d + e + f
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTALT, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_A, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_B, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_C, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_D, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_E, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x08, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_F, .value = EV_MAKE },
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTALT, .value = EV_BREAK },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );

    // // shift + z
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_MAKE },
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_Z, .value = EV_MAKE },
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    // shift + a
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_A, .value = EV_MAKE },
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );

    // shift was pressed but then released, so we shouldn't see a shift key in this case
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_MAKE },
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.addData (
        input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK },
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );

    test.run();

    if ( test.m_E != nullptr ) {
        try {
            std::rethrow_exception ( test.m_E );
        } catch ( const IoException& ex ) {
            EXPECT_EQ ( ex.what(), std::string ( "No more data to read." ) );
            EXPECT_EQ ( ex.code(), 42 );
        } catch ( ... ) {
            FAIL() << "Expected an IoException but got something else.";
        }
    } else {
        FAIL() << "Expected an Exception but didn't get one.";
    }

    EXPECT_EQ ( test.m_ReceivedData.size(), test.m_ExpectedData.size() );

    for ( int i = 0; i < test.m_ExpectedData.size(); ++i ) {
        EXPECT_EQ ( test.m_ReceivedData.front(), test.m_ExpectedData.front() );
        test.m_ReceivedData.pop();
        test.m_ExpectedData.pop();
    }
}
