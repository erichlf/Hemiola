#pragma once

#include "HID.h"

#include <cstdint>
#include <string>
#include <vector>

namespace hemiola
{
    /*!
     * @brief simple class handling communication with input device
     */
    class OutputHID : public HID
    {
    public:
        /*!
         * @copydoc HID::HID(const std::string&)
         */
        explicit OutputHID ( const std::string& device  = "/dev/hidg0" );
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
         * @param data byte data for the keypress to send to HID output
         * @throw IoException if we are unable to write to device
         * @assumption device has been opened for writing
         */
        void write ( const std::vector<uint8_t>& data );
    };
}
