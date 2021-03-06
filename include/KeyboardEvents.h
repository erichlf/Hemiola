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
#pragma once

#include "InputHID.h"
#include "KeyTable.h"

#include <linux/input.h>

#include <exception>
#include <functional>
#include <memory>

namespace hemiola
{
    using KeyArray = std::array<uint8_t, 6>;
    /*!
     * @brief struct describing the current key press
     */
    struct KeyReport
    {
        /*!
         * @modifier hex codes (bit 0 is L CTRL, bit 1 is L SHIFT, bit 2 is L ALT, bit 3 is L GUI,
         *                      bit 4 is R CTRL, bit 5 is R SHIFT, bit 6 is R ALT, and bit 7 is R
         *                      GUI).
         */
        uint8_t modifiers;
        /*!
         * @brief list of keys pressed with modifier (6 allowed)
         */
        KeyArray keys {};
    };

    /*!
     * @brief comparison operator for KeyReport
     */
    bool operator== ( const KeyReport& lhs, const KeyReport& rhs );

    /*!
     * @brief class for capturing keyboard events
     */
    class KeyboardEvents
    {
    public:
        explicit KeyboardEvents ( std::shared_ptr<KeyTable> keyTable );
        KeyboardEvents ( std::shared_ptr<KeyTable> keyTable, std::shared_ptr<InputHID> device );
        KeyboardEvents ( const KeyboardEvents& ) = delete;
        KeyboardEvents ( KeyboardEvents&& ) = delete;
        KeyboardEvents& operator= ( const KeyboardEvents& ) = delete;
        KeyboardEvents& operator= ( KeyboardEvents&& ) = delete;
        ~KeyboardEvents() = default;

        /*!
         * @brief begin capturing keys
         * @param onEvent function which will handle any key capture events
         * @param onError function which will handle any errors that arise
         */
        void capture ( std::function<void ( KeyReport, std::string )> onEvent,
                       std::function<void ( std::exception_ptr )> onError );

    private:
        /*!
         * @brief read key event
         * @param event the key event captured from the keyboard
         * @return true if key event an event was captured succesfully
         * @throw IOError if an event was not able to be read from the keyboard
         */
        bool getEvent ( input_event& event ) const;

        /*!
         * @brief function that translates key press into KeyState
         * @param event the key event to process
         * @post m_KeyReport and m_KeyRep will contain data corresponding to event
         */
        void updateKeyState ( const input_event& event );

        /*!
         * @brief the current key press
         */
        KeyReport m_KeyReport;

        /*!
         * @brief the string representation of the current key press/release
         * @note non-modifers use a blank ("") representation for a release
         */
        std::string m_KeyRep;

        /*
         * @brief object containing the key map
         */
        std::shared_ptr<KeyTable> m_KeyTable;

        /*!
         * @brief input device we are capturing keys from
         */
        std::shared_ptr<InputHID> m_InputHID;
    };
}  // namespace hemiola
