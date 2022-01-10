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
#include "FakeInputHID.h"
#include "KeyboardEvents.h"
#include "Utils.h"

#include <gtest/gtest.h>
#include <linux/input.h>

#include <exception>
#include <iostream>
#include <queue>
#include <string>

namespace hemiola
{
    /*!
     * @brief test class for simulating key presses
     */
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

        /*!
         * @brief release the given key
         * @param key the key to be pressed
         * @param report the expected result
         * @post the simulated key press and the expected result are saved to test data
         */
        void press ( const unsigned short key, const KeyReport& report )
        {
            addData ( input_event { .type = EV_KEY, .code = key, .value = EV_MAKE }, report );
        }

        /*!
         * @brief release the given key
         * @param key the key to be released
         * @param report the expected result
         * @post the simulated key release and the expected result are saved to test data
         */
        void release ( const unsigned short key, const KeyReport& report )
        {
            addData ( input_event { .type = EV_KEY, .code = key, .value = EV_BREAK }, report );
        }

        /*!
         * @brief run the simulated key presses
         * @post simulated key press data is received
         */
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

        /*!
         * @brief check that the correct exception was received at the end of our simulation
         */
        void checkException()
        {
            if ( m_E != nullptr ) {
                try {
                    std::rethrow_exception ( m_E );
                } catch ( const IoException& ex ) {
                    EXPECT_EQ ( ex.what(), std::string ( "No more data to read." ) );
                    EXPECT_EQ ( ex.code(), 42 );
                } catch ( ... ) {
                    FAIL() << "Expected an IoException but got something else.";
                }
            } else {
                FAIL() << "Expected an Exception but didn't get one.";
            }
        }

        /*!
         * @brief check if the simulated key presses and the expected results agree
         */
        void checkData()
        {
            EXPECT_EQ ( m_ReceivedData.size(), m_ExpectedData.size() );

            for ( int i = 0; i < m_ExpectedData.size(); ++i ) {
                EXPECT_EQ ( m_ReceivedData.front(), m_ExpectedData.front() );
                m_ReceivedData.pop();
                m_ExpectedData.pop();
            }
        }

    private:
        /*!
         * @brief add data a key press to the test data
         * @param event the key press to simulate
         * @param report the expected result
         * @post the simulated key press/release and the expected result are saved to test data
         */
        void addData ( input_event event, KeyReport report )
        {
            m_Data.push ( event );
            m_ExpectedData.push ( report );
        }

        /*!
         * @brief the expected results from simulated key presses
         */
        std::queue<KeyReport> m_ExpectedData;
        /*!
         * @brief the received results from simulated key presses
         */
        std::queue<KeyReport> m_ReceivedData;
        /*!
         * @brief any exception received during the simulation
         */
        std::exception_ptr m_E;
        /*!
         * @brief key presses to be simulated
         */
        std::queue<input_event> m_Data;
    };
}

TEST ( KeyboardEventTest, KeyPressTest )
{
    using namespace hemiola;

    hemiola::Test test;

    // invalid scanCode
    test.press ( 1029, KeyReport {} );
    test.release ( 1029, KeyReport {} );

    // no modifiers
    test.press (
        KEY_5,
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x22, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_5, KeyReport {} );

    test.press (
        KEY_M,
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x10, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_M, KeyReport {} );

    test.press (
        KEY_BACKSLASH,
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x31, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_BACKSLASH, KeyReport {} );

    test.press (
        KEY_F12,
        KeyReport { .modifiers = 0x00, .keys = KeyArray { 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_F12, KeyReport {} );

    test.run();
    test.checkException();
    test.checkData();
}

TEST ( KeyboardEventTest, ModifierTest )
{
    using namespace hemiola;

    hemiola::Test test;

    // ctrl + alt + c
    test.press (
        KEY_LEFTALT,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.press ( KEY_RIGHTCTRL,
                 KeyReport { .modifiers = 0x04 | 0x10,
                             .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.press ( KEY_C,
                 KeyReport { .modifiers = 0x04 | 0x10,
                             .keys = KeyArray { 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_C,
                   KeyReport { .modifiers = 0x04 | 0x10,
                               .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release (
        KEY_LEFTALT,
        KeyReport { .modifiers = 0x10, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_RIGHTCTRL, KeyReport {} );

    // alt + a + b + c + d + e + f
    test.press (
        KEY_LEFTALT,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.press (
        KEY_A,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.press (
        KEY_B,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x00, 0x00, 0x00, 0x00 } } );
    test.press (
        KEY_C,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x00, 0x00, 0x00 } } );
    test.press (
        KEY_D,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x00, 0x00 } } );
    test.press (
        KEY_E,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x08, 0x00 } } );
    test.press (
        KEY_F,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 } } );
    // no more than 6 keys can be pressed at once
    test.press (
        KEY_G,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 } } );
    test.release (
        KEY_G,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 } } );
    // now release our keys
    test.release (
        KEY_A,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x05, 0x06, 0x07, 0x08, 0x09 } } );
    test.release (
        KEY_B,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x06, 0x07, 0x08, 0x09 } } );
    test.release (
        KEY_C,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x07, 0x08, 0x09 } } );
    test.release (
        KEY_D,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x08, 0x09 } } );
    test.release (
        KEY_E,
        KeyReport { .modifiers = 0x04, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x09 } } );
    test.release ( KEY_F, KeyReport { .modifiers = 0x04, .keys = KeyArray {} } );
    test.release ( KEY_LEFTALT, KeyReport {} );

    // // shift + z
    test.press (
        KEY_LEFTSHIFT,
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.press (
        KEY_Z,
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_Z, KeyReport { .modifiers = 0x02, .keys = KeyArray {} } );

    // shift + a
    test.press (
        KEY_A,
        KeyReport { .modifiers = 0x02, .keys = KeyArray { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 } } );
    test.release ( KEY_A, KeyReport { .modifiers = 0x02, .keys = KeyArray {} } );
    test.release ( KEY_LEFTSHIFT, KeyReport {} );

    // shift was pressed but then released
    test.press ( KEY_LEFTSHIFT, KeyReport { .modifiers = 0x02, .keys = KeyArray {} } );
    test.release ( KEY_LEFTSHIFT, KeyReport {} );

    test.run();
    test.checkException();
    test.checkData();
}
