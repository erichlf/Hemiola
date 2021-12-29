#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace hemiola
{
    class Anagram
    {
    public:
        Anagram() = default;
        Anagram ( const Anagram& ) = delete;
        Anagram ( Anagram&& ) = delete;
        Anagram& operator= ( const Anagram& ) = delete;
        Anagram& operator= ( Anagram&& ) = delete;
        ~Anagram() = default;

        /*!
         * @brief insert word into data structure
         */
        void insert ( const std::wstring& word );

        /*!
         * @brief lookup up anagrams for given string
         * @param letters string to find anagrams for
         * @return vector containing anagrams
         */
        std::vector<std::wstring> lookup ( std::wstring letters ) const;

    private:
        std::unordered_map<std::wstring, std::vector<std::wstring>> m_Anagrams;
    };
}
