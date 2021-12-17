#include "Anagram.h"

#include <algorithm>
#include <iostream>
#include <locale>

using namespace hemiola;

namespace
{
    /*
     * @brief sorts a string lexicographically and changes it to lowercase
     * @param string to sort and change to lowercase
     * @return the string sorted and changed to lowercase
     */
    std::string sortToLower ( const std::string& word )
    {
        std::locale loc;

        if ( word.size() == 0 ) return word;

        // sort will not compare things that only have one letter, but it still needs to be lowercase
        if ( word.size() == 1 ) return std::string { std::tolower ( word [0], loc ) };

        const auto comparatorMod = [&loc] ( auto& a, auto& b ) {
            a = std::tolower ( a, loc );
            b = std::tolower ( b, loc );

            return a < b;
        };

        auto sorted { word };

        std::sort ( sorted.begin(), sorted.end(), comparatorMod );

        return sorted;
    }
}

void hemiola::Anagram::insert ( const std::string& word )
{
    // keys are strings that are sorted lexicographically
    const auto letters = sortToLower ( word );

    if ( m_Anagrams.count ( letters ) != 0 ) {
        m_Anagrams.at ( letters ).push_back ( word );
    } else {
        m_Anagrams [letters] = { word };
    }
}

std::vector<std::string> hemiola::Anagram::lookup ( std::string letters ) const
{
    if ( letters.size() == 0 ) return {};

    // sort our letters so that we can lookup our key
    letters = sortToLower ( letters );

    if ( m_Anagrams.count ( letters ) != 0 ) {
        return m_Anagrams.at ( letters );
    } else {
        return {};
    }
}
