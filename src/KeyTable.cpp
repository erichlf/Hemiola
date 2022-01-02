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
#include "KeyTable.h"

/* a lot of this code was stolen from https://github.com/kernc/logkeys but then modified */

#include "KeyboardEvents.h"
#include "Utils.h"

#include <bitset>
#include <cassert>
#include <sstream>
#include <string>

using namespace hemiola;

void hemiola::KeyTable::determineSystemKeymap()
{
    // custom map will be used; erase existing US keymapping
    m_CharKeys.clear();
    m_ShiftKeys.clear();
    m_AltgrKeys.clear();

    m_CharKeys.resize ( 49 );
    m_ShiftKeys.resize ( 49 );
    m_AltgrKeys.resize ( 49 );

    // get keymap from dumpkeys
    // if one knows of a better, more portable way to get wchar_t-s from symbolic keysym-s from
    // `dumpkeys` or `xmodmap` or another, PLEASE LET ME KNOW! kthx
    std::stringstream ss,
        dump (
            execute ( COMMAND_STR_DUMPKEYS ) );  // see example output after i.e. `loadkeys slovene`
    std::string line;

    unsigned int i = 0;  // code
    int index;
    int utf8code;  // utf-8 code of keysym answering code i

    while ( std::getline ( dump, line ) ) {
        ss.clear();
        ss.str ( "" );
        utf8code = 0;

        // replace any U+#### with 0x#### for easier parsing
        index = line.find ( "U+", 0 );
        while ( static_cast<std::string::size_type> ( index ) != std::string::npos ) {
            line [index] = '0';
            line [index + 1] = 'x';
            index = line.find ( "U+", index );
        }

        if ( !isCodeValid ( ++i ) )
            break;  // only ever map codes up to 128 (currently N_KEYS_DEFINED are used)
        if ( !isCharKey ( i ) )
            continue;  // only map character keys of keyboard

        assert ( line.size() > 0 );
        if ( line [0] == 'k' ) {  // if line starts with 'code'
            index = toCharKeysIndex ( i );

            ss << &line [14];  // 1st keysym starts at index 14 (skip "code XXX = ")
            ss >> std::hex >> utf8code;
            // 0XB00CLUELESS: 0xB00 is added to some keysyms that are preceeded with '+'; I
            // don't really know why; see `man keymaps`; `man loadkeys` says numeric keysym
            // values aren't to be relied on, orly?
            if ( line [14] == '+' && ( utf8code & 0xB00 ) )
                utf8code ^= 0xB00;
            m_CharKeys [index] = static_cast<wchar_t> ( utf8code );

            // if there is a second keysym column, assume it is a shift column
            if ( ss >> std::hex >> utf8code ) {
                if ( line [14] == '+' && ( utf8code & 0xB00 ) )
                    utf8code ^= 0xB00;
                m_ShiftKeys [index] = static_cast<wchar_t> ( utf8code );
            }

            // if there is a third keysym column, assume it is an altgr column
            if ( ss >> std::hex >> utf8code ) {
                if ( line [14] == '+' && ( utf8code & 0xB00 ) )
                    utf8code ^= 0xB00;
                m_AltgrKeys [index] = static_cast<wchar_t> ( utf8code );
            }

            continue;
        }

        // else if line starts with 'shift i'
        index = toCharKeysIndex ( --i );
        ss << &line [21];  // 1st keysym starts at index 21 (skip "\tshift\tcode XXX = " or
                           // "\taltgr\tcode XXX = ")
        ss >> std::hex >> utf8code;
        if ( line [21] == '+' && ( utf8code & 0xB00 ) )
            utf8code ^= 0xB00;  // see line 0XB00CLUELESS

        if ( line [1] == 's' )  // if line starts with "shift"
            m_ShiftKeys [index] = static_cast<wchar_t> ( utf8code );
        if ( line [1] == 'a' )  // if line starts with "altgr"
            m_AltgrKeys [index] = static_cast<wchar_t> ( utf8code );
    }  // while (getline(dump, line))
}

bool hemiola::KeyTable::isCharKey ( unsigned int code ) const
{
    if ( code > m_CharOrFunc.size() ) {
        return false;
    }
    return ( m_CharOrFunc [code] == 'c' );
}

bool hemiola::KeyTable::isFuncKey ( unsigned int code ) const
{
    if ( code > m_CharOrFunc.size() ) {
        return false;
    }
    return ( m_CharOrFunc [code] == 'f' );
}

bool hemiola::KeyTable::isUsedKey ( unsigned int code ) const
{
    if ( code > m_CharOrFunc.size() ) {
        return false;
    }
    return ( m_CharOrFunc [code] != '_' );
}

int hemiola::KeyTable::toCharKeysIndex ( unsigned int code ) const
{
    int index = -1;                            // not character code
    if ( code >= KEY_1 && code <= KEY_EQUAL )  // codes 2-13: US keyboard: 1, 2, ..., 0, -, =
        index = code - 2;
    if ( code >= KEY_Q && code <= KEY_RIGHTBRACE )  // codes 16-27: q, w, ..., [, ]
        index = code - 4;
    if ( code >= KEY_A && code <= KEY_GRAVE )  // codes 30-41: a, s, ..., ', `
        index = code - 6;
    if ( code >= KEY_BACKSLASH && code <= KEY_SLASH )  // codes 43-53: \, z, ..., ., /
        index = code - 7;

    if ( code == KEY_102ND )
        index = 47;  // key right to the left of 'Z' on US layout

    return index;
}

inline int hemiola::KeyTable::toFuncKeysIndex ( unsigned int code ) const
{
    int index = -1;         // not character code
    if ( code == KEY_ESC )  // 1
        index = 0;
    if ( code >= KEY_BACKSPACE && code <= KEY_TAB )  // 14-15
        index = code - 13;
    if ( code >= KEY_ENTER && code <= KEY_LEFTCTRL )  // 28-29
        index = code - 25;
    if ( code == KEY_LEFTSHIFT )
        index = code - 37;                              // 42
    if ( code >= KEY_RIGHTSHIFT && code <= KEY_KPDOT )  // 54-83
        index = code - 48;
    if ( code >= KEY_F11 && code <= KEY_F12 )  // 87-88
        index = code - 51;
    if ( code >= KEY_KPENTER && code <= KEY_DELETE )  // 96-111
        index = code - 58;
    if ( code == KEY_PAUSE )  // 119
        index = code - 65;
    if ( code >= KEY_LEFTMETA && code <= KEY_COMPOSE )  // 125-127
        index = code - 70;
    return index;
}

uint8_t hemiola::KeyTable::scanToHex ( unsigned int code ) const
{
    try {
        return m_HexValues.at ( code );
    } catch ( ... ) {
        std::cerr << "Unknown key code: " << code << "\n";
        return 0x00;
    }
}

uint8_t hemiola::KeyTable::modToHex ( unsigned int code ) const
{
    try {
        return m_ModiferHex.at ( code );
    } catch ( ... ) {
        std::cerr << "Unknown key code: " << code << "\n";
        return 0x00;
    }
}

// std::optional<wchar_t> hemiola::KeyTable::handleScanCode ( unsigned int code, const KeyState&
// keyState ) const
// {
//     std::optional<wchar_t> wch;
//     if ( isCharKey ( code ) ) {
//         if ( keyState.altgr ) {
//             wch = altgrKeys ( code );
//             if ( wch == L'\0' ) {
//                 if ( keyState.shift ) {
//                     wch = shiftKeys ( code );
//                 } else {
//                     wch = charKeys ( code );
//                 }
//             }
//         } else if ( keyState.capslock
//                     && iswalpha ( charKeys ( code ) ) ) {  // only bother with capslock if alpha
//             if ( keyState.shift ) {  // capslock and shift cancel each other
//                 wch = charKeys ( code );
//             } else {
//                 wch = shiftKeys ( code );
//             }

//             if ( wch == L'\0' ) {
//                 wch = charKeys ( code );
//             }
//         } else if ( keyState.shift ) {
//             wch = shiftKeys ( code );
//             if ( wch == L'\0' ) {
//                 wch = charKeys ( code );
//             }
//         } else {  // neither altgr nor shift are in use, this is a normal char
//             wch = charKeys ( code );
//         }
//     }

//     return wch;
// }
