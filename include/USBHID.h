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
#pragma once

#include "OutputHID.h"

#include <string>

namespace hemiola
{
    // forward declarations
    struct KeyReport;

    /*!
     * @brief simple class handling communication with input device
     */
    class USBHID : public OutputHID
    {
    public:
        /*!
         * @copydoc HID::HID(const std::string&)
         */
        explicit USBHID ( const std::string& device = "/dev/hidg0" );
        USBHID ( const USBHID& ) = delete;
        USBHID ( USBHID&& ) = delete;
        USBHID& operator= ( const USBHID& ) = delete;
        USBHID& operator= ( USBHID&& ) = delete;
        ~USBHID();

        /*!
         * @brief write scan code to hid
         * @param report byte data for the keypress to send to HID output
         * @throw IoException if we are unable to write to device
         * @assumption device has been opened for writing
         */
        void write ( const KeyReport& report ) const override;
    };
}  // namespace hemiola
