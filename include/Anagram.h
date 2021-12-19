#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace hemiola
{
    /*!
     * @brief class for determining the anagrams of a given string
     */
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
        void insert ( const std::string& word );

        /*!
         * @brief lookup up anagrams for given string
         * @param letters string to find anagrams for
         * @return vector containing anagrams
         */
        std::vector<std::string> lookup ( std::string letters ) const;

    private:
        std::unordered_map<std::string, std::vector<std::string>> m_Anagrams;
    };
}  // namespace hemiola
