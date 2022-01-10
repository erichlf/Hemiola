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
#include "Anagram.h"

#include <algorithm>
#include <cwctype>
#include <iostream>
#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>
#include <utility>

using namespace hemiola;

using ListOfDicts = std::vector<std::pair<std::string, std::string>>;

namespace
{
    /*
     * @brief sorts a string lexicographically and changes it to lowercase
     * @param string to sort and change to lowercase
     * @return the string sorted and changed to lowercase
     */
    std::wstring sortTowlower ( const std::wstring& word )
    {
        if ( word.size() == 0 ) {
            return word;
        }

        // sort will not compare things that only have one letter, but it still needs to be
        // lowercase
        if ( word.size() == 1 ) {
            return std::wstring { static_cast<wchar_t> ( std::towlower ( word [0] ) ) };
        }

        const auto comparatorMod = [] ( auto& a, auto& b ) {
            a = std::towlower ( a );
            b = std::towlower ( b );

            return a < b;
        };

        auto sorted { word };

        std::sort ( sorted.begin(), sorted.end(), comparatorMod );

        return sorted;
    }

    [[maybe_unused]] ListOfDicts::const_iterator findDictionary ( const std::string& dict )
    {
        ListOfDicts dictList;
        nuspell::search_default_dirs_for_dicts ( dictList );
        auto dictIter = nuspell::find_dictionary ( dictList, dict );
        return dictIter;
    }
}

void hemiola::Anagram::insert ( const std::wstring& word )
{
    // keys are strings that are sorted lexicographically
    const auto letters = sortTowlower ( word );

    if ( m_Anagrams.count ( letters ) != 0 ) {
        m_Anagrams.at ( letters ).push_back ( word );
    } else {
        m_Anagrams [letters] = { word };
    }
}

std::vector<std::wstring> hemiola::Anagram::lookup ( std::wstring letters ) const
{
    if ( letters.size() == 0 )
        return {};

    // sort our letters so that we can lookup our key
    letters = sortTowlower ( letters );

    if ( m_Anagrams.count ( letters ) != 0 ) {
        return m_Anagrams.at ( letters );
    } else {
        return {};
    }
}
