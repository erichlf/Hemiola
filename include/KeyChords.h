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
#pragma once

#include "KeyTable.h"

#include <linux/input.h>

#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace hemiola
{
    struct Hasher
    {
        inline std::size_t operator() ( const std::set<unsigned int>& a ) const
        {
            std::size_t h = 0;

            for ( auto e : a ) {
                h ^= std::hash<int> {}( e ) + 0x9e3779b9 + ( h << 6 ) + ( h >> 2 );
            }
            return h;
        }
    };

    using ChordMap = std::unordered_map<std::set<unsigned int>, std::string, Hasher>;

    /*!
     * @brief class which contains the map of chords to words
     */
    class KeyChords
    {
    public:
        explicit KeyChords ( std::shared_ptr<KeyTable> keyTable );
        ~KeyChords() = default;

        /*!
         * @brief Given a chord return the corresponding string. If the chord doesn't exist return
         * the original chord.
         * @param
         */
        std::string getWord ( const std::string& chord ) const;

        /*!
         * @brief Builds our chord map from user input
         */
        void buildMap();

        /*!
         * @brief Translate the given chord to a word
         * @param chord The chord to translate
         * @return The word corresponding to the chord
         */
        std::string getWord ( const std::string& chord );

    private:
        /*!
         * @brief split chord into individual characters
         * @param chord the chord to split into characters
         * @return a set representing the chord
         */
        std::set<unsigned int> parseChord ( std::string chord );

        /*!
         * map of chords to words
         * @TODO: read this in from a yaml file
         */
        ChordMap m_ChordMap;

        /*!
         * Key representing the special input dup
         */
        unsigned int m_Dup;
        /*!
         * Key representing the special input plural
         */
        unsigned int m_Plural;
        /*!
         * Key representing the special input past
         */
        unsigned int m_Past;

        /*!
         * Object holding the key table
         */
        std::shared_ptr<KeyTable> m_KeyTable;
    };
}
