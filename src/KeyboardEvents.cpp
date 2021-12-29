/* a lot of this code was stolen from https://github.com/kernc/logkeys but then modified */

#include "KeyboardEvents.h"

#include "Exceptions.h"
#include "KeyTable.h"
#include "Utils.h"

#include <cassert>
#include <cwctype>
#include <iostream>
#include <string>

#include <linux/input.h>

using namespace hemiola;

hemiola::KeyboardEvents::KeyboardEvents ( std::shared_ptr<KeyTable> keyTable )
    : m_KeyTable ( std::move ( keyTable ) )
    , m_InputHID ( std::make_shared<InputHID>() )
{}

hemiola::KeyboardEvents::KeyboardEvents ( std::shared_ptr<KeyTable> keyTable, std::shared_ptr<InputHID> device )
    : m_KeyTable ( std::move ( keyTable ) )
    , m_InputHID ( std::move ( device ) )
{}

void hemiola::KeyboardEvents::capture (
    std::function<void ( KeyState )> passThrough,
    std::function<void ( std::variant<wchar_t, unsigned short> )> onEvent,
    std::function<void ( std::exception_ptr )> onError )
{
    try {
        while ( updateKeyState() ) {
            passThrough ( m_KeyState );  // send the scan code directly to the output
            captureEvent ( onEvent );  // process the scan code
        }
    } catch ( ... ) {
        std::cerr << "Input device closed unexpectedly.\n";
        m_InputHID->close();
        onError ( std::current_exception() );
    }
}

void hemiola::KeyboardEvents::captureEvent (
    std::function<void ( std::variant<wchar_t, unsigned short> )> onEvent )
{
    unsigned short scanCode = m_KeyState.event.code;

    if ( !m_KeyState.scanCodeOk ) {  // keycode out of range, log error
        std::cerr << "Keycode out of range: <E-" << scanCode << ">\n";
        onEvent ( scanCode );

        return;
    }

    if ( m_KeyTable->isCharKey ( scanCode ) ) {
        assert ( m_KeyState.key != std::nullopt );  // this shouldn't happen if scanCode is a character
        onEvent ( m_KeyState.key.value() );
    } else if ( m_KeyTable->isFuncKey ( scanCode ) || scanCode == KEY_SPACE
                || scanCode == KEY_TAB ) {
        // we don't want to send altgr or shift keys since m_KeyState.key will have that
        if ( !m_KeyTable->isModifier ( scanCode ) ) {
            onEvent ( scanCode );
        }
    } else {
        std::cerr << "Unknown keycode: <E-" << scanCode << ">\n";
        onEvent ( scanCode );
    }
}

bool hemiola::KeyboardEvents::updateKeyState()
{
    try {
        m_InputHID->read ( m_KeyState.event );
    } catch ( ... ) {
        std::cerr << "Connection to keyboard seems to have been lost while updating key state.\n";
        throw;
    }

    if ( m_KeyState.event.type != EV_KEY ) {
        return updateKeyState();  // keyboard events are always of type EV_KEY
    }

    unsigned short scanCode
        = m_KeyState.event.code;  // the key code of the pressed key (some codes are from "scan code
                                  // set 1", some are different (see <linux/input.h>)

    m_KeyState.repeatEnd = false;
    if ( m_KeyState.event.value == EV_REPEAT ) {
        m_KeyState.repeats++;
        return true;
    } else if ( m_KeyState.event.value == EV_BREAK ) {
        if ( scanCode == KEY_LEFTSHIFT || scanCode == KEY_RIGHTSHIFT ) {
            m_KeyState.shift = false;
        } else if ( scanCode == KEY_RIGHTALT ) {
            m_KeyState.altgr = false;
        } else if ( scanCode == KEY_LEFTCTRL || scanCode == KEY_RIGHTCTRL ) {
            m_KeyState.ctrl = false;
        } else if ( scanCode == KEY_LEFTALT || scanCode == KEY_RIGHTALT ) {
            m_KeyState.alt = false;
        } else if ( scanCode == KEY_LEFTMETA || scanCode == KEY_RIGHTMETA ) {
            // press and release of meta needs to be handled differently
            m_KeyState.meta = false;
            m_KeyState.key = m_KeyTable->handleScanCode ( scanCode, m_KeyState );
            return true;
        }

        m_KeyState.repeatEnd = m_KeyState.repeats > 0;
        if ( m_KeyState.repeatEnd ) {
            return true;
        } else {
            return updateKeyState();
        }
    }
    m_KeyState.repeats = 0;

    m_KeyState.scanCodeOk = m_KeyTable->isCodeValid ( scanCode );
    if ( !m_KeyState.scanCodeOk ) {
        return true;
    }

    m_KeyState.key = 0;

    if ( m_KeyState.event.value != EV_MAKE ) {
        return updateKeyState();
    }

    std::optional<wchar_t> wch;
    switch ( scanCode ) {
        case KEY_CAPSLOCK:
            m_KeyState.capslock = !m_KeyState.capslock;
            break;
        case KEY_LEFTSHIFT:
        case KEY_RIGHTSHIFT:
            m_KeyState.shift = true;
            break;
        case KEY_RIGHTALT:
            m_KeyState.altgr = true;
            break;
        case KEY_LEFTCTRL:
        case KEY_RIGHTCTRL:
            m_KeyState.ctrl = true;
            break;
        case KEY_LEFTALT:  // KEY_LEFTALT == KEY_RIGHTALT
            m_KeyState.alt = true;
            break;
        case KEY_LEFTMETA:
        case KEY_RIGHTMETA:
            m_KeyState.meta = true;
            break;
        default:
            wch = m_KeyTable->handleScanCode ( scanCode, m_KeyState );
    }
    m_KeyState.key = wch;

    return true;
}
