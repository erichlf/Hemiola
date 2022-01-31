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

std::string hemiola::KeyTable::charKeys ( const unsigned int code ) const
{
    try {
        return m_CharKeys.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Invalid character key code: {}", code );
        return "";
    }
}

std::string hemiola::KeyTable::modKeys ( const unsigned int code ) const
{
    try {
        return m_ModKeys.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Invalid modifier key code: {}", code );
        return "";
    }
}

bool hemiola::KeyTable::isModifier ( const std::string& key ) const
{
    for ( auto modKey : m_ModKeys ) {
        if ( ( beginModKey ( modKey.first ) == key ) || ( endModKey ( modKey.first ) == key ) ) {
            return true;
        }
    }

    return false;
}

bool hemiola::KeyTable::isModifierPair ( const std::string& first, const std::string& second ) const
{
    // one of the keys is not a modifier so clearly there can't be a match
    if ( !isModifier ( first ) || !isModifier ( second ) ) {
        return false;
    }

    // determine which type of modifier first is and set the match
    std::string match{};
    for ( auto el : m_ModKeys ) {
        if ( beginModKey ( el.first ) == first ) {
            match = endModKey ( el.first );
            break;
        } else if ( endModKey ( el.first ) == first ) {
            match = beginModKey ( el.first );
            break;
        }
    }

    return second == match;
}

std::string hemiola::KeyTable::beginModKey ( const unsigned int code ) const
{
    auto key = modKeys ( code );
    return key.empty() ? key : fmt::format ( m_BeginModifierFmt, key );
}

std::string hemiola::KeyTable::endModKey ( const unsigned int code ) const
{
    auto key = modKeys ( code );
    return key.empty() ? key : fmt::format ( m_EndModifierFmt, key );
}

uint8_t hemiola::KeyTable::scanToHex ( const unsigned int code ) const
{
    try {
        return m_HexValues.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Unknown key code: {}", code );
        return 0x00;
    }
}

uint8_t hemiola::KeyTable::modToHex ( const unsigned int code ) const
{
    try {
        return m_ModiferHex.at ( code );
    } catch ( ... ) {
        LOG ( ERROR, "Unknown modifier key code: {}", code );
        return 0x00;
    }
}
