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
#include "Hemiola.h"

#include "Logger.h"

#include <functional>

using namespace hemiola;

hemiola::Hemiola::Hemiola ( std::shared_ptr<KeyTable> keyTable )
    : m_Captured {}
    , m_Anagrammer {}
    , m_KeyTable { std::move ( keyTable ) }
    , m_DontCapture { false }
{}

void hemiola::Hemiola::addKey ( const std::string& key )
{
    LOG ( DEBUG, "KEY: {}", key );
    if ( key.empty() ) {
        return;
    }

    auto notShiftOrAltGr = [this] ( const auto key ) {
        return ( key != m_KeyTable->beginModKey ( KEY_RIGHTALT )
                 && key != m_KeyTable->beginModKey ( KEY_RIGHTSHIFT )
                 && key != m_KeyTable->beginModKey ( KEY_LEFTSHIFT )
                 && key != m_KeyTable->endModKey ( KEY_RIGHTALT )
                 && key != m_KeyTable->endModKey ( KEY_RIGHTSHIFT )
                 && key != m_KeyTable->endModKey ( KEY_LEFTSHIFT ) );
    };

    // key is for the beginning of a non-capturable modifier sequence
    if ( m_KeyTable->isBeginModifier ( key ) && notShiftOrAltGr ( key ) ) {
        m_DontCapture = true;
        LOG ( DEBUG, "DONT CAPTURE: {}, KEY: {}", m_DontCapture, key );
        return;
    } else if ( m_KeyTable->isEndModifier ( key ) && notShiftOrAltGr ( key ) ) {
        // key is for the end of a non-capturable modifier sequence
        m_DontCapture = false;
        LOG ( DEBUG, "DONT CAPTURE: {}, KEY: {}", m_DontCapture, key );
        return;
    }

    // not capturing or no key sent so nothing to do
    if ( m_DontCapture ) {
        return;
    }

    if ( key == m_KeyTable->charKeys ( KEY_SPACE ) || key == m_KeyTable->charKeys ( KEY_ENTER ) ) {
        // TODO: use Anagrammer to process the string capture
        LOG ( DEBUG, "Word Boundary found with word {}" );
        m_Captured.clear();
        return;
    }

    if ( key == m_KeyTable->charKeys ( KEY_BACKSPACE ) ) {
        deleteKey();
        return;
    }

    m_Captured.push_back ( key );
}

void hemiola::Hemiola::deleteKey()
{
    // predicate for checking if a key is not a modifier
    auto notModifier
        = [this] ( const std::string& key ) { return !m_KeyTable->isModifier ( key ); };

    // get the largest sequence such that the begin and end reverse iterators are the same type of
    // modifier
    using riter = decltype ( m_Captured.rbegin() );
    std::function<void ( riter&, riter& )> surroundingModifiers;
    surroundingModifiers = [this, &surroundingModifiers] ( riter& begin, riter& end ) {
        auto newBegin = std::next ( begin, -1 );
        auto newEnd = std::next ( end );
        if ( m_KeyTable->isModifierPair ( *newBegin, *newEnd ) ) {
            // advance our iterators and then try to expand
            begin = newBegin;
            end = newEnd;
            surroundingModifiers ( begin, end );
        }
    };

    // find the last key capture that isn't a modifier
    auto begin = std::find_if ( m_Captured.rbegin(), m_Captured.rend(), notModifier );
    auto end = begin;

    // no key found that isn't a modifier
    if ( begin == m_Captured.rend() ) {
        return;
    }

    // expand to remove all surrounding modifiers and stop expanding once we have no more modifier
    // pairs
    surroundingModifiers ( begin, end );

    // now actually remove the key
    m_Captured.erase ( std::next ( end ).base(), begin.base() );
}
