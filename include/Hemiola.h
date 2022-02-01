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

#include "Anagrammer.h"
#include "KeyTable.h"

#include <memory>
#include <string>
#include <vector>

namespace hemiola
{
    /*!
     * @brief class which process characters and determines the intended word
     */
    class Hemiola
    {
    public:
        explicit Hemiola ( std::shared_ptr<KeyTable> keyTable );
        Hemiola ( const Hemiola& ) = delete;
        Hemiola ( Hemiola&& ) = delete;
        Hemiola& operator= ( const Hemiola& ) = delete;
        Hemiola& operator= ( Hemiola&& ) = delete;
        ~Hemiola() = default;

        /*!
         * @brief add the key press to the captured string
         * @param key String representation of key press
         * @post key press is added to captured keys, and if a word boundary is detected the
         * captured string word is processed for anagrams
         */
        void addKey ( const std::string& key );

        /*!
         * @brief return a vector of the currently captured keys
         * @return vector of captured keys
         */
        const std::vector<std::string>& captured() const { return m_Captured; };

    private:
        /*!
         * @brief remove the most recent key in m_Captured, including any surrounding modifiers
         * @post the most recent key and any surrounding modifiers are removed from m_Captured
         */
        void deleteKey();

        /*!
         * @brief the currently captured text
         */
        std::vector<std::string> m_Captured;

        /*!
         * @brief determines anagrams of m_Captured
         */
        Anagrammer m_Anagrammer;

        /*!
         * @brief key table describing character representations
         */
        std::shared_ptr<KeyTable> m_KeyTable;

        /*!
         * @brief flag indicating if we should be capturing text
         */
        bool m_DontCapture;
    };

}
