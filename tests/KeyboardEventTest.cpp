#include <gtest/gtest.h>

#include "Exceptions.h"
#include "FakeDevice.h"
#include "KeyboardEvents.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <linux/input.h>

TEST ( KeyboardEventTest, KeyPressTest )
{
    auto device = std::make_shared<hemiola::FakeDevice>();

    std::vector<input_event> data;

    device->setData ( data );

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
}
