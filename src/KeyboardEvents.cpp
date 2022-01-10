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

#include "KeyboardEvents.h"

#include "Exceptions.h"
#include "KeyTable.h"
#include "Utils.h"

#include <linux/input.h>

#include <bitset>
#include <cassert>
#include <cwctype>
#include <iostream>
#include <string>

using namespace hemiola;

hemiola::KeyboardEvents::KeyboardEvents ( std::shared_ptr<KeyTable> keyTable )
    : KeyboardEvents ( keyTable, std::make_shared<InputHID>() )
{}

hemiola::KeyboardEvents::KeyboardEvents ( std::shared_ptr<KeyTable> keyTable,
                                          std::shared_ptr<InputHID> device )
    : m_KeyReport {}
    , m_KeyTable { std::move ( keyTable ) }
    , m_InputHID ( std::move ( device ) )
{}

void hemiola::KeyboardEvents::capture ( std::function<void ( KeyReport )> onEvent,
                                        std::function<void ( std::exception_ptr )> onError )
{
    try {
        while ( updateKeyState() ) {
            onEvent ( m_KeyReport );  // send the scan code directly to the output
        }
    } catch ( ... ) {
        std::cerr << "Input device closed unexpectedly.\n";
        m_InputHID->close();
        onError ( std::current_exception() );
    }
}

bool hemiola::KeyboardEvents::updateKeyState()
{
    input_event event {};
    try {
        m_InputHID->read ( event );
    } catch ( ... ) {
        std::cerr << "Connection to keyboard seems to have been lost while updating key state.\n";
        throw;
    }

    if ( event.type != EV_KEY ) {
        return updateKeyState();  // keyboard events are always of type EV_KEY
    }

    // the key code of the pressed key
    unsigned short scanCode = event.code;

    if ( event.value == EV_REPEAT ) {
        return true;
    } else if ( event.value == EV_BREAK ) {
        // we need to check if the key is a modifier first for this to work correctly
        if ( m_KeyTable->isModifier ( scanCode ) ) {  // turn off the current modifier
            m_KeyReport.modifiers &= ~m_KeyTable->modToHex ( scanCode );
        } else if ( m_KeyTable->isKeyValid ( scanCode ) ) {
            const auto scanHex { m_KeyTable->scanToHex ( scanCode ) };
            // find the key and set to 0
            for ( auto& code : m_KeyReport.keys ) {
                if ( code == scanHex ) {
                    code = 0x00;
                    break;
                }
            }
        }

        return true;
    }

    // this was not a key press so move on
    if ( event.value != EV_MAKE ) {
        return updateKeyState();
    }

    // we need to check if the key is a modifier first for this to work correctly
    if ( m_KeyTable->isModifier ( scanCode ) ) {
        m_KeyReport.modifiers |= m_KeyTable->modToHex ( scanCode );
    } else if ( m_KeyTable->isKeyValid ( scanCode ) ) {
        const auto scanHex { m_KeyTable->scanToHex ( scanCode ) };
        if ( m_KeyReport.keys [0] != scanHex && m_KeyReport.keys [1] != scanHex
             && m_KeyReport.keys [2] != scanHex && m_KeyReport.keys [3] != scanHex
             && m_KeyReport.keys [4] != scanHex && m_KeyReport.keys [5] != scanHex ) {
            // add current key press to the list of key presses, and don't overwrite
            for ( auto& code : m_KeyReport.keys ) {
                if ( code == 0x00 ) {
                    code = scanHex;
                    break;
                }
            }
        }
    }

    return true;
}

bool hemiola::operator== ( const KeyReport& lhs, const KeyReport& rhs )
{
    return lhs.modifiers == rhs.modifiers
           && ( lhs.keys [0] == rhs.keys [0] && lhs.keys [1] == rhs.keys [1]
                && lhs.keys [2] == rhs.keys [2] && lhs.keys [3] == rhs.keys [3]
                && lhs.keys [4] == rhs.keys [4] && lhs.keys [5] == rhs.keys [5] );
}
