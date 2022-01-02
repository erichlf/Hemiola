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

#include <queue>

#include "InputHID.h"

namespace hemiola
{
    /*!
     * @brief Class which spoof InputHID for testing purposes
     */
    class FakeInputHID : public InputHID
    {
    public:
        FakeInputHID();
        FakeInputHID ( const FakeInputHID& ) = delete;
        FakeInputHID ( FakeInputHID&& ) = delete;
        FakeInputHID& operator= ( const FakeInputHID& ) = delete;
        FakeInputHID& operator= ( FakeInputHID&& ) = delete;
        ~FakeInputHID() = default;

        /*!
         * @brief open device for reading
         * @post device is opened and can be read from
         */
        virtual void open() override;

        /*!
         * @brief close device
         * @post device has been closed and can no longer be read from
         */
        virtual void close() override;

        /*!
         * @brief read event from device
         * @param event input_event that we are going to save
         * @throw IoException if there is no more data to read
         * @assumption device has been opened for reading
         */
        virtual void read ( input_event& event ) override;

        /*!
         * @brief sets the data to send to whatever calls read
         * @param events the events to stream
         */
        void setData ( const std::queue<input_event>& events );

    private:
        /*!
         * @brief data to be output from read
         */
        std::queue<input_event> m_Data;
    };
}  // namespace hemiola
