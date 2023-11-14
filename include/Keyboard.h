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

#include <string>

// forward declaration
struct input_event;

namespace hemiola
{
    /*!
     * @brief simple class handling communication with input device
     */
    class Keyboard : public InputHID
    {
    public:
        Keyboard();
        Keyboard ( const Keyboard& ) = delete;
        Keyboard ( Keyboard&& ) = delete;
        Keyboard& operator= ( const Keyboard& ) = delete;
        Keyboard& operator= ( Keyboard&& ) = delete;
        ~Keyboard() = default;

        /*!
         * @copydoc HID::open
         */
        void open() override;

        /*!
         * @brief read event from device
         * @param event input_event that we are going to save
         * @throw IoException if we are unable to read from device
         * @assumption device has been opened for reading
         */
        void read ( input_event& event ) override;

    private:
        /*!
         * @brief look up keyboard
         * @throw KeyboardException if a keyboard cannot be found
         */
        std::string getKeyboard();
    };
}  // namespace hemiola
