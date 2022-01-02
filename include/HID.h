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

#include <string>

namespace hemiola
{
    /*!
     * @brief simple class handling communication with input device
     */
    class HID
    {
    public:
        HID();
        /*
         * @brief CTOR to setup our hid
         * @param the device location
         */
        explicit HID ( const std::string& device );
        HID ( const HID& ) = delete;
        HID ( HID&& ) = delete;
        HID& operator= ( const HID& ) = delete;
        HID& operator= ( HID&& ) = delete;
        virtual ~HID();

        /*!
         * @brief open device for reading
         * @throw an IoException if connection can't be established
         * @post device is opened and can be read from
         */
        virtual void open() = 0;

        /*!
         * @brief close device
         * @post device has been closed and can no longer be read from
         */
        virtual void close();

    protected:
        /*!
         * @copydoc HID::open()
         * @param perms permissions used for opening this device see
         *        https://linux.die.net/man/3/open
         */
        void open ( int perms );

        /*!
         * @brief file descriptor for our keyboard
         */
        int m_HIDId;

        /*!
         * @brief the device we are reading from
         */
        std::string m_HIDString;

        /*
         * @brief flag indicating if the device has been opened
         */
        bool m_Opened;
    };
}  // namespace hemiola
