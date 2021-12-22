#pragma once

#include <string>

#include <linux/input.h>

namespace hemiola
{
    /*!
     * @brief simple class handling communication with input device
     */
    class Device
    {
    public:
        Device();
        Device ( const Device& ) = delete;
        Device ( Device&& ) = delete;
        Device& operator= ( const Device& ) = delete;
        Device& operator= ( Device&& ) = delete;
        ~Device();

        /*!
         * @brief open device for reading
         * @throw an IoException if connection can't be established
         * @post device is opened and can be read from
         */
        virtual void open();

        /*!
         * @brief close device
         * @post device has been closed and can no longer be read from
         */
        virtual void close();

        /*!
         * @brief read event from device
         * @param event input_event that we are going to save
         * @throw IoException if we are unable to read from device
         * @assumption device has been opened for reading
         */
        virtual void read ( input_event& event );

    private:
        /*!
         * @brief look up keyboard
         * @throw KeyboardException if a keyboard cannot be found
         */
        std::string getInputDevice();

        /*!
         * @brief file descriptor for our keyboard
         */
        int m_DeviceId;

        /*!
         * @brief the device we are reading from
         */
        std::string m_DeviceString;

    protected:
        /*
         * @brief flag indicating if the device has been opened
         */
        bool m_Opened;
    };
}
