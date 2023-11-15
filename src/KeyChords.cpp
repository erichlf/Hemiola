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
#include "KeyChords.h"

#include "Logger.h"

#include <yaml-cpp/yaml.h>

#include <queue>

using namespace hemiola;

const static std::string CONFIG { "config/settings.yml" };

const static char DEFAULT_DUP { '=' };
const static char DEFAULT_PLURAL { ';' };
const static char DEFAULT_PAST { ',' };
const static char SEPARATOR { '+' };

hemiola::KeyChords::KeyChords ( std::shared_ptr<KeyTable> keyTable )
    : m_ChordMap {}
    , m_Dup { KEY_RESERVED }
    , m_Plural { KEY_RESERVED }
    , m_Past { KEY_RESERVED }
    , m_KeyTable ( keyTable )
{}

void hemiola::KeyChords::buildMap()
{
    auto config = YAML::LoadFile ( CONFIG );

    if ( config ["dup"] ) {
        m_Dup = m_KeyTable->getKeyCode ( config ["dup"].as<std::string>() );
    }

    if ( config ["plural"] ) {
        m_Plural = m_KeyTable->getKeyCode ( config ["plural"].as<std::string>() );
    }

    if ( config ["past"] ) {
        m_Past = m_KeyTable->getKeyCode ( config ["past"].as<std::string>() );
    }

    if ( m_Dup == KEY_RESERVED ) {
        LOG ( DEBUG, "Using default dup key" );
        m_Dup = DEFAULT_DUP;
    }

    if ( m_Plural == KEY_RESERVED ) {
        LOG ( DEBUG, "Using default plural key" );
        m_Plural = DEFAULT_PLURAL;
    }

    if ( m_Past == KEY_RESERVED ) {
        LOG ( DEBUG, "Using default past key" );
        m_Past = DEFAULT_PAST;
    }

    if ( config ["chords"] && config ["chords"].IsMap() ) {
        for ( auto it = config ["chords"].begin(); it != config ["chords"].end(); ++it ) {
            const auto key = it->first;
            const auto value = it->second;
            if ( key.Type() == YAML::NodeType::Scalar && value.Type() == YAML::NodeType::Scalar ) {
                // TODO parse the value
                auto chord = parseChord ( value.as<std::string>() );
                if ( !chord.empty() && m_ChordMap.count ( chord ) == 0 ) {
                    m_ChordMap [chord] = key.as<std::string>();
                } else if ( !chord.empty() ) {
                    LOG ( WARN,
                          "The provided chord ({}) clashes with another chord ({}).",
                          key.as<std::string>(),
                          m_ChordMap.at ( chord ) );
                }
            } else {
                LOG ( WARN, "Nested chords are not supported." );
            }
        }
    }
}

std::set<unsigned int> hemiola::KeyChords::parseChord ( std::string chord )
{
    // remove all white space
    chord.erase ( std::remove_if ( chord.begin(), chord.end(), ::isspace ), chord.end() );

    std::queue<std::size_t> seps;
    // search through string until no separator is found
    for ( std::size_t indx = chord.find ( SEPARATOR, 0 ); indx != std::string::npos;
          indx = chord.find ( SEPARATOR, indx + 1 ) ) {
        seps.push ( indx );
    }

    // we assume chords are written with letters first and then special characters
    std::size_t end = seps.empty() ? chord.size() : seps.front() + 1;
    seps.pop();

    std::set<unsigned int> chordSet;
    // empty spaces have been removed so don't worry about them
    for ( size_t i = 0; i < end; ++i ) {
        std::string token { chord.at ( i ) };
        chordSet.insert ( m_KeyTable->getKeyCode ( token ) );
    }

    while ( !seps.empty() ) {
        auto begin = end;
        end = seps.front() != std::string::npos ? seps.front() + 1 : chord.size();
        seps.pop();

        auto special = std::string { chord.begin() + begin, chord.begin() + end };

        if ( special == m_KeyTable->charKeys ( m_Dup ) ) {
            chordSet.insert ( m_Dup );
        } else if ( special == m_KeyTable->charKeys ( m_Plural ) ) {
            chordSet.insert ( m_Plural );
        } else if ( special == m_KeyTable->charKeys ( m_Past ) ) {
            chordSet.insert ( m_Past );
        } else {
            LOG ( WARN, "Unknown special in chord: {}", special );
        }
    }

    return chordSet;
}

std::string hemiola::KeyChords::getWord ( const std::string& chord )
{
    auto chordSet = parseChord ( chord );
    return ( chordSet.empty() || m_ChordMap.count ( chordSet ) > 0 ) ? m_ChordMap [chordSet]
                                                                     : chord;
}
