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

    std::vector<input_event> data;
    std::vector<std::variant<wchar_t, unsigned short>> expectedData;

    // invalid scanCode
    data.emplace_back ( input_event { .type = EV_KEY, .code = 129, .value = EV_MAKE } );
    expectedData.emplace_back ( static_cast<unsigned short> ( 129  ) );

    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_5, .value = EV_MAKE } );
    expectedData.emplace_back ( wchar_t ( '5' ) );

    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_M, .value = EV_MAKE } );
    expectedData.emplace_back ( wchar_t ( 'm' ) );

    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_BACKSLASH, .value = EV_MAKE } );
    expectedData.emplace_back ( wchar_t ( '\\' ) );

    // ctrl + c
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK } );
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_RIGHTCTRL, .value = EV_MAKE } );
    expectedData.emplace_back ( static_cast<unsigned short> ( KEY_RIGHTCTRL  ) );
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_C, .value = EV_MAKE } );
    expectedData.emplace_back ( wchar_t ( 'c' ) );

    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_F12, .value = EV_MAKE } );
    expectedData.emplace_back ( static_cast<unsigned short> ( KEY_F12  ) );

    // shift + z
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK } );
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_Z, .value = EV_MAKE } );
    expectedData.emplace_back ( wchar_t ( 'Z' ) );
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_MAKE } );

    // shift was pressed but then released, so we shouldn't see a shift key in this case
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_BREAK } );
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_LEFTSHIFT, .value = EV_MAKE } );
    data.emplace_back ( input_event { .type = EV_KEY, .code = KEY_EQUAL, .value = EV_MAKE } );
    expectedData.emplace_back ( wchar_t ( '=' ) );

    device->setData ( data );
    // receivedData will be the reverse of this data, because I didn't use a queue
    std::reverse ( expectedData.begin(), expectedData.end() );

    auto keyTable = std::make_shared<hemiola::KeyTable>();

    hemiola::KeyboardEvents keys ( keyTable, device );

    // data that will be captured by keys
    std::vector<std::variant<wchar_t, unsigned short>> receivedData;
    auto onEvent = [&receivedData] ( std::variant<wchar_t, unsigned short> event ) {
        receivedData.push_back ( event );
    };

    // the exception that will be thrown by keys
    std::exception_ptr e;
    auto onError = [&e] ( std::exception_ptr exc ) { e = exc; };

    // normally this would be run in its own thread
    keys.capture ( onEvent, onError );

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

    EXPECT_EQ ( receivedData.size(), expectedData.size() );
    for ( int i = 0; i < expectedData.size(); ++i ) {
        EXPECT_EQ ( receivedData.at ( i ), expectedData.at ( i ) );
    }
}
