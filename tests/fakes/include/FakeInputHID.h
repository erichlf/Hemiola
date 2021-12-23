#pragma once

#include "InputHID.h"

#include <vector>

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
        void setData ( const std::vector<input_event>& events );

    private:
        /*!
         * @brief data to be output from read
         */
        std::vector<input_event> m_Data;
    };
}
