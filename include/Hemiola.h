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
#include "KeyTable.h"
#include "OutputHID.h"

#include <chrono>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace hemiola
{
    /*!
     * @brief class which process characters and determines the intended word
     */
    class Hemiola
    {
    public:
        using TimePoint = std::chrono::steady_clock::time_point;

        Hemiola ( std::shared_ptr<KeyTable> keyTable,
                  std::shared_ptr<KeyChords> keyChords,
                  std::shared_ptr<OutputHID> output );
        Hemiola ( const Hemiola& ) = delete;
        Hemiola ( Hemiola&& ) = delete;
        Hemiola& operator= ( const Hemiola& ) = delete;
        Hemiola& operator= ( Hemiola&& ) = delete;
        ~Hemiola();

        /*!
         * @brief add the key press to the captured string
         * @param key representation of key press
         * @post if key presses occur in succession in less than the given threshold the captured
         * keys are converted to a word and out put to the output device
         */
        void addKey ( const unsigned int key );

        /*!
         * @brief return a set of the currently captured keys
         * @return set of captured keys
         */
        const std::unordered_map<unsigned int, TimePoint>& captured() const
        {
            return m_Captured;
        };

        /*!
         * @brief Function which runs the timer and grabs keychords
         */
        void run();

    private:
        /*!
         * @brief remove the most recent key in m_Captured, including any surrounding modifiers
         * @post the most recent key and any surrounding modifiers are removed from m_Captured
         */
        void deleteKey();

        /*!
         * @brief the currently captured text
         */
        std::unordered_map<unsigned int, TimePoint> m_Captured;

        /*!
         * @brief key table describing character representations
         */
        std::shared_ptr<KeyTable> m_KeyTable;

        /*!
         * @brief map between chords and words
         */
        std::shared_ptr<KeyChords> m_KeyChords;

        /*!
         * @brief device to out put keys to
         */
        std::shared_ptr<OutputHID> m_Output;

        /*!
         * @brief Sequence of modifiers received.
         */
        std::vector<unsigned int> m_ModSequence;

        // Time threshold for outputting the key codes
        std::chrono::milliseconds m_TimeThreshold;

        // Thread that runs the timer loop
        std::thread m_TimerThread;

        // Mutex to protect access to the shared data structures
        std::mutex m_Mutex;
    };

}
