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

    // for now we aren't going to do anything with the data
    auto onEvent = [] ( std::variant<wchar_t, unsigned short> ) {};

    // the exception that will be thrown by keys
    std::exception_ptr e;
    auto onError = [&e] ( std::exception_ptr exc ) { e = exc; };

    auto passThrough = [&keys, &output, &onError] ( KeyState keyState ) {
        uint8_t modifier = 0x00;
        if ( keyState.ctrl ) {
            modifier |= keys->modToHex ( KEY_RIGHTCTRL );
        }
        if ( keyState.shift && !keyState.capslock ) {
            modifier |= keys->modToHex ( KEY_RIGHTSHIFT );
        } else if ( !keyState.shift && keyState.capslock ) {
            if ( keyState.event.code != KEY_ENTER ) {
                modifier |= keys->modToHex ( KEY_CAPSLOCK );
            }
        }
        if ( keyState.alt ) {
            modifier |= keys->modToHex ( KEY_LEFTALT );
        }
        if ( keyState.altgr ) {
            modifier |= keys->modToHex ( KEY_RIGHTALT );
        }
        if ( keyState.meta ) {
            modifier |= keys->modToHex ( KEY_LEFTMETA );
        }
        try {
            output->write ( std::vector<uint8_t> {
                modifier,
                0x00,
                keys->scanToHex ( keyState.event.code ),
                0x00,
                0x00,
                0x00,
                0x00,
                0x00 } );
            output->write (
                std::vector<uint8_t> { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } );
        } catch ( ... ) {
            onError ( std::current_exception() );
        }
    };

    eventHandler.capture ( passThrough, onEvent, onError );
}
