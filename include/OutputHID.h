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

#include <cstdint>
#include <string>
#include <vector>

#include "HID.h"

namespace hemiola
{
    // forward declarations
    struct KeyReport;

    /*!
     * @brief simple class handling communication with input device
     */
    class OutputHID : public HID
    {
    public:
        /*!
         * @copydoc HID::HID(const std::string&)
         */
        explicit OutputHID ( const std::string& device = "/dev/hidg0" );
        OutputHID ( const OutputHID& ) = delete;
        OutputHID ( OutputHID&& ) = delete;
        OutputHID& operator= ( const OutputHID& ) = delete;
        OutputHID& operator= ( OutputHID&& ) = delete;
        ~OutputHID() = default;

        /*!
         * @copydoc HID::open
         */
        virtual void open();

        /*!
         * @brief write scan code to hid
         * @param report byte data for the keypress to send to HID output
         * @throw IoException if we are unable to write to device
         * @assumption device has been opened for writing
         */
        void write ( const KeyReport& report ) const;
    };
}  // namespace hemiola
