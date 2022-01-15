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
#include "Logger.h"
#include "Utils.h"

#include <bitset>
#include <cassert>
#include <sstream>
#include <string>

using namespace hemiola;

// void hemiola::KeyTable::determineSystemKeymap()
// {
//     // custom map will be used; erase existing US keymapping
//     m_CharKeys.clear();
//     m_ShiftKeys.clear();
//     m_AltgrKeys.clear();

//     m_CharKeys.resize ( 49 );
//     m_ShiftKeys.resize ( 49 );
//     m_AltgrKeys.resize ( 49 );

//     // get keymap from dumpkeys
//     // if one knows of a better, more portable way to get char-s from symbolic keysym-s from
//     // `dumpkeys` or `xmodmap` or another, PLEASE LET ME KNOW! kthx
//     std::stringstream ss,
//         dump (
//             execute ( COMMAND_STR_DUMPKEYS ) );  // see example output after i.e. `loadkeys
//             slovene`
//     std::string line;

//     unsigned int i = 0;  // code
//     int index;
//     int utf8code;  // utf-8 code of keysym answering code i

//     while ( std::getline ( dump, line ) ) {
//         ss.clear();
//         ss.str ( "" );
//         utf8code = 0;

//         // replace any U+#### with 0x#### for easier parsing
//         index = line.find ( "U+", 0 );
//         while ( static_cast<std::string::size_type> ( index ) != std::string::npos ) {
//             line [index] = '0';
//             line [index + 1] = 'x';
//             index = line.find ( "U+", index );
//         }

//         if ( !isCodeValid ( ++i ) )
//             break;  // only ever map codes up to 128 (currently N_KEYS_DEFINED are used)
//         if ( !isCharKey ( i ) )
//             continue;  // only map character keys of keyboard

//         assert ( line.size() > 0 );
//         if ( line [0] == 'k' ) {  // if line starts with 'code'
//             index = toCharKeysIndex ( i );

//             ss << &line [14];  // 1st keysym starts at index 14 (skip "code XXX = ")
//             ss >> std::hex >> utf8code;
//             // 0XB00CLUELESS: 0xB00 is added to some keysyms that are preceeded with '+'; I
//             // don't really know why; see `man keymaps`; `man loadkeys` says numeric keysym
//             // values aren't to be relied on, orly?
//             if ( line [14] == '+' && ( utf8code & 0xB00 ) )
//                 utf8code ^= 0xB00;
//             m_CharKeys [index] = static_cast<char> ( utf8code );

//             // if there is a second keysym column, assume it is a shift column
//             if ( ss >> std::hex >> utf8code ) {
//                 if ( line [14] == '+' && ( utf8code & 0xB00 ) )
//                     utf8code ^= 0xB00;
//                 m_ShiftKeys [index] = static_cast<char> ( utf8code );
//             }

//             // if there is a third keysym column, assume it is an altgr column
//             if ( ss >> std::hex >> utf8code ) {
//                 if ( line [14] == '+' && ( utf8code & 0xB00 ) )
//                     utf8code ^= 0xB00;
//                 m_AltgrKeys [index] = static_cast<char> ( utf8code );
//             }

//             continue;
//         }

//         // else if line starts with 'shift i'
//         index = toCharKeysIndex ( --i );
//         ss << &line [21];  // 1st keysym starts at index 21 (skip "\tshift\tcode XXX = " or
//                            // "\taltgr\tcode XXX = ")
//         ss >> std::hex >> utf8code;
//         if ( line [21] == '+' && ( utf8code & 0xB00 ) )
//             utf8code ^= 0xB00;  // see line 0XB00CLUELESS

//         if ( line [1] == 's' )  // if line starts with "shift"
//             m_ShiftKeys [index] = static_cast<char> ( utf8code );
//         if ( line [1] == 'a' )  // if line starts with "altgr"
//             m_AltgrKeys [index] = static_cast<char> ( utf8code );
//     }  // while (getline(dump, line))
// }

std::string hemiola::KeyTable::charKeys ( unsigned int code ) const
{
    try {
        return m_CharKeys.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Invalid character key code: {}", code );
        return "";
    }
}

std::string hemiola::KeyTable::modKeys ( unsigned int code ) const
{
    try {
        return m_ModKeys.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Invalid modifier key code: {}", code );
        return "";
    }
}

uint8_t hemiola::KeyTable::scanToHex ( unsigned int code ) const
{
    try {
        return m_HexValues.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Unknown key code: {}", code );
        return 0x00;
    }
}

uint8_t hemiola::KeyTable::modToHex ( unsigned int code ) const
{
    try {
        return m_ModiferHex.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Unknown modifier key code: {}", code );
        return 0x00;
    }
}

// std::optional<char> hemiola::KeyTable::handleScanCode ( unsigned int code, const KeyState&
// keyState ) const
// {
//     std::optional<char> wch;
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
