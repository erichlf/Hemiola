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
#include "Hemiola.h"

#include "KeyReport.h"
#include "Logger.h"

#include <fmt/ranges.h>

#include <functional>

using namespace hemiola;

hemiola::Hemiola::Hemiola ( std::shared_ptr<KeyTable> keyTable,
                            std::shared_ptr<KeyChords> keyChords,
                            std::shared_ptr<OutputHID> output )
    : m_Captured {}
    , m_KeyTable { std::move ( keyTable ) }
    , m_KeyChords { std::move ( keyChords ) }
    , m_Output { output }
    , m_ModSequence {}
    , m_TimeThreshold { 300 }
{}

void hemiola::Hemiola::addKey ( const unsigned int key )
{
    LOG ( DEBUG, "KEY: {}", key );
    // key release
    if ( key == m_KeyTable->keyRelease() ) {
        return;
    }

    auto notShiftOrAltGr = [this] ( const auto key ) -> bool {
        return key != KEY_RIGHTALT && key != KEY_RIGHTSHIFT && key != KEY_LEFTSHIFT;
    };

    // check if key is a modifier, if it is then check if it is a release or a press. If it is a
    // press add it to the list of modifiers in use, otherwise remove it from the modifier list.
    if ( m_KeyTable->isModifier ( key ) && notShiftOrAltGr ( key ) ) {
        bool existed = false;
        for ( auto it = m_ModSequence.begin(); it != m_ModSequence.end(); ++it ) {
            if ( key == *it ) {
                existed = true;
                m_ModSequence.erase ( it );
                break;
            }
        }

        if ( !existed ) {
            m_ModSequence.push_back ( key );
        }

        return;
    }

    // A modifier is pressed so don't capture.
    if ( !m_ModSequence.empty() ) {
        return;
    }

    if ( key == KEY_SPACE || key == KEY_ENTER ) {
        m_Captured.clear();
        return;
    }

    if ( key == KEY_BACKSPACE ) {
        // deleteKey();
        return;
    }

    std::lock_guard<std::mutex> lock ( m_Mutex );
    m_Captured [key] = std::chrono::steady_clock::now();
}

// void hemiola::Hemiola::deleteKey()
// {
//     // predicate for checking if a key is not a modifier
//     auto notModifier
//         = [this] ( const unsigned int key ) { return !m_KeyTable->isModifier ( key ); };

//     // get the largest sequence such that the begin and end reverse iterators are the same type
//     of
//     // modifier
//     using riter = decltype ( m_Captured.rbegin() );
//     std::function<void ( riter&, riter& )> surroundingModifiers;
//     surroundingModifiers = [this, &surroundingModifiers] ( riter& begin, riter& end ) {
//         auto newBegin = std::next ( begin, -1 );
//         auto newEnd = std::next ( end );
//         if ( m_KeyTable->isModifier ( *newBegin ) && m_KeyTable->isModifier ( *newEnd )
//              && *newBegin == *newEnd ) {
//             // advance our iterators and then try to expand
//             begin = newBegin;
//             end = newEnd;
//             surroundingModifiers ( begin, end );
//         }
//     };

//     // find the last key capture that isn't a modifier
//     auto begin = std::find_if ( m_Captured.rbegin(), m_Captured.rend(), notModifier );
//     auto end = begin;

//     // no key found that isn't a modifier
//     if ( begin == m_Captured.rend() ) {
//         return;
//     }

//     // expand to remove all surrounding modifiers and stop expanding once we have no more
//     modifier
//     // pairs
//     surroundingModifiers ( begin, end );

//     // now actually remove the key
//     m_Captured.erase ( std::next ( end ).base(), begin.base() );
// }

void hemiola::Hemiola::run()
{
    // Create a thread that runs the timer loop
    m_TimerThread = std::thread ( [&] {
        while ( true ) {
            // Lock the mutex to access the shared data structures
            std::lock_guard<std::mutex> lock ( m_Mutex );
            // Iterate over the key codes in the combination
            std::string chord;
            for ( const auto& [keyCode, timestamp] : m_Captured ) {
                // Check if the time threshold has passed since the key code was added
                if ( std::chrono::steady_clock::now() - timestamp >= m_TimeThreshold ) {
                    // add the key code to the chord if the time threshold has passed
                    chord += m_KeyTable->charKeys ( keyCode );
                    // Remove the key code from the combination
                    m_Captured.erase ( keyCode );
                }
            }
            // TODO: delete chord before sending word to output

            // loop over word and send it to output
            for ( const auto& charKey : m_KeyChords->getWord ( chord ) ) {
                KeyReport report;
                auto keyCode = m_KeyTable->getKeyCode ( std::string ( 1, charKey ) );
                auto keyHex = m_KeyTable->scanToHex ( keyCode );
                report.setKey ( keyHex );
                m_Output->write ( report );
                report.unsetKey ( keyHex );
            }

            // Sleep for a short time before checking the timestamps again
            std::this_thread::sleep_for ( std::chrono::milliseconds ( 10 ) );
        }
    } );
}
