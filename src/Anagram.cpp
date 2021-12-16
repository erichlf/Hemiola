#include "Anagram.h"

#include <algorithm>

using namespace hemiola;

void hemiola::Anagram::insert ( const std::string& word )
{
    auto letters { word };

    // keys are strings that are sorted lexicographically
    std::sort ( letters.begin(), letters.end() );

    if ( m_Anagrams.count ( letters ) != 0 ) {
        m_Anagrams.at ( letters ).push_back ( word );
    } else {
        m_Anagrams.at ( letters ) = { word };
    }
}

std::vector<std::string> hemiola::Anagram::lookup ( std::string letters ) const
{
    if ( letters.size() == 0 ) return {};

    // sort our letters so that we can lookup our key
    std::sort ( letters.begin(), letters.end() );

    if ( m_Anagrams.count ( letters ) != 0 ) {
        return m_Anagrams.at ( letters );
    } else {
        return {};
    }
}
