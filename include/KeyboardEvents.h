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

#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <variant>

#include <linux/input.h>

#include "InputHID.h"
#include "KeyTable.h"

namespace hemiola
{
    /*!
     * @brief struct which represents a keyboard event
     */
    struct KeyState
    {
        std::optional<wchar_t> key;
        unsigned int repeats;  // count_repeats differs from the actual number of repeated
        // characters! afaik, only the OS knows how these two values are
        // related (by respecting configured repeat speed and delay)
        bool repeatEnd;
        input_event event;
        bool scanCodeOk;
        bool capslock = false;
        bool shift = false;
        bool alt = false;
        bool altgr = false;
        bool ctrl = false;
        bool meta = false;
    };

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
         * @param passThrough function which will send data straight to output device
         * @param onEvent function which will handle any key capture events
         * @param onError function which will handle any errors that arise
         */
        void capture ( std::function<void ( KeyState )> passThrough,
                       std::function<void ( std::variant<wchar_t, unsigned short> )> onEvent,
                       std::function<void ( std::exception_ptr )> onError );

    private:
        /*
         * @brief function that translates key press into KeyState
         * @return true if key event was processed false if connection seems to be lost
         */
        bool updateKeyState();

        /*!
         * @brief handle the current event
         * @param onEvent callback for when new event has been processed
         * @post events are sent to the callback
         * @note in the case of character keys a wchar_t is sent to the callback, however in
         *       the case of a modifier, e.g. shift, nothing is given to the callback as those
         *       keys act on the next character. For all other function keys the scan code is
         *       send to the callback.
         */
        void captureEvent ( std::function<void ( std::variant<wchar_t, unsigned short> )> onEvent );

        /*!
         * @brief the state of the key press
         */
        KeyState m_KeyState;

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
