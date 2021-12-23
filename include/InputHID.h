#pragma once

#include "HID.h"

#include <string>

// forward declaration
struct input_event;

namespace hemiola
{
    /*!
     * @brief simple class handling communication with input device
     */
    class InputHID : public HID
    {
    public:
        InputHID();
        InputHID ( const InputHID& ) = delete;
        InputHID ( InputHID&& ) = delete;
        InputHID& operator= ( const InputHID& ) = delete;
        InputHID& operator= ( InputHID&& ) = delete;
        ~InputHID() = default;

        /*!
         * @copydoc HID::open
         */
        virtual void open();

        /*!
         * @brief read event from device
         * @param event input_event that we are going to save
         * @throw IoException if we are unable to read from device
         * @assumption device has been opened for reading
         */
        virtual void read ( input_event& event );

    protected:
        /*!
         * @brief look up keyboard
         * @throw KeyboardException if a keyboard cannot be found
         */
        std::string getInputHID();
    };
}
