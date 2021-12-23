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
}
