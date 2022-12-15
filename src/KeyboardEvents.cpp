/*
  MIT License
  Copyright (c) 2021-2022 Erich L Foster
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
#include "Logger.h"
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
    , m_KeyRep {}
    , m_KeyTable { std::move ( keyTable ) }
    , m_InputHID ( std::move ( device ) )
{}

void hemiola::KeyboardEvents::capture ( std::function<void ( KeyReport, unsigned int )> onEvent,
                                        std::function<void ( std::exception_ptr )> onError )
{
    try {
        input_event event {};
        while ( getEvent ( event ) ) {
            updateKeyState ( event );           // process the captured event
            onEvent ( m_KeyReport, m_KeyRep );  // send the scan code directly to the output
        }
    } catch ( ... ) {
        LOG ( ERROR, "An error occurred while reading keyboard event" );
        m_InputHID->close();
        onError ( std::current_exception() );
    }
}

bool hemiola::KeyboardEvents::getEvent ( input_event& event ) const
{
    try {
        m_InputHID->read ( event );
    } catch ( ... ) {
        LOG ( ERROR, "Connection to keyboard seems to have been lost while updating key state" );
        throw;
    }
    LOG ( DEBUG,
          "(event.type, event.value, event.code) = ({}, {}, {})",
          event.type,
          event.value,
          event.code );

    return true;
}

void hemiola::KeyboardEvents::updateKeyState ( const input_event& event )
{
    // reset our key
    m_KeyRep = m_KeyTable->keyRelease();

    if ( event.type != EV_KEY ) {
        return;  // keyboard events are always of type EV_KEY
    }

    // the key code of the pressed key
    unsigned short scanCode = event.code;

    if ( event.value == EV_REPEAT ) {
        return;
    } else if ( event.value == EV_BREAK ) {
        // we need to check if the key is a modifier first for this to work correctly
        if ( m_KeyTable->isScanModifier ( scanCode ) ) {  // turn off the current modifier
            const auto scanHex { m_KeyTable->modToHex ( scanCode ) };
            m_KeyReport.unsetModifier ( scanHex );
            m_KeyRep = scanCode;
        } else if ( m_KeyTable->isKeyValid ( scanCode ) ) {
            const auto scanHex { m_KeyTable->scanToHex ( scanCode ) };
            m_KeyReport.unsetKey ( scanHex );
        }

        return;
    }

    // this was not a key press so move on
    if ( event.value != EV_MAKE ) {
        return;
    }

    // we need to check if the key is a modifier first for this to work correctly
    if ( m_KeyTable->isModifier ( scanCode ) ) {
        LOG ( DEBUG, "MAKE Modifer: {} -> {}", scanCode, m_KeyTable->modToHex ( scanCode ) );
        const auto scanHex { m_KeyTable->modToHex ( scanCode ) };
        m_KeyReport.setModifier ( scanHex );
        m_KeyRep = scanCode;
    } else if ( m_KeyTable->isKeyValid ( scanCode ) ) {
        const auto scanHex { m_KeyTable->scanToHex ( scanCode ) };
        LOG ( DEBUG, "MAKE key: {} -> {}", scanCode, scanHex );
        if ( !m_KeyReport.setKey ( scanHex ) ) {
            m_KeyRep = scanCode;
        }
    }
}
