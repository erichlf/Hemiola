#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <variant>

#include <linux/input.h>

#include "InputHID.h"
#include "KeyTable.h"

namespace hemiola
{
    /*!
     * @brief struct which represents a keyboard event
     */
    struct KeyState
    {
        wchar_t key;
        unsigned int repeats;  // count_repeats differs from the actual number of repeated
        // characters! afaik, only the OS knows how these two values are
        // related (by respecting configured repeat speed and delay)
        bool repeatEnd;
        input_event event;
        bool scanCodeOk;
        bool capslock = false;
        bool shift = false;
        bool altgr = false;
        bool ctrl = false;  // used for identifying Ctrl+C / Ctrl+D
    };

    /*!
     * @brief class for capturing keyboard events
     */
    class KeyboardEvents
    {
    public:
        explicit KeyboardEvents ( std::shared_ptr<KeyTable> keyTable );
        KeyboardEvents ( std::shared_ptr<KeyTable> keyTable, std::shared_ptr<InputHID> device );
        KeyboardEvents ( const KeyboardEvents& ) = delete;
        KeyboardEvents ( KeyboardEvents&& ) = delete;
        KeyboardEvents& operator= ( const KeyboardEvents& ) = delete;
        KeyboardEvents& operator= ( KeyboardEvents&& ) = delete;
        ~KeyboardEvents() = default;

        /*!
         * @brief begin capturing keys
         * @param passThrough function which will send raw scan codes to output device
         * @param onEvent function which will handle any key capture events
         * @param onError function which will handle any errors that arise
         */
        void capture ( std::function<void ( const unsigned short )> passThrough,
                       std::function<void ( std::variant<wchar_t, unsigned short> )> onEvent,
                       std::function<void ( std::exception_ptr )> onError );

    private:
        /*
         * @brief function that translates key press into KeyState
         * @param passThrough function which will send raw scan codes to output device
         * @return true if key event was processed false if connection seems to be lost
         */
        bool updateKeyState ( std::function<void ( const unsigned short )> passThrough );

        /*!
         * @brief handle the current event
         * @param onEvent callback for when new event has been processed
         * @post events are sent to the callback
         * @note in the case of character keys a wchar_t is sent to the callback, however in
         *       the case of a modifier, e.g. shift, nothing is given to the callback as those
         *       keys act on the next character. For all other function keys the scan code is
         *       send to the callback.
         */
        void captureEvent ( std::function<void ( std::variant<wchar_t, unsigned short> )> onEvent );

        /*!
         * @brief the state of the key press
         */
        KeyState m_KeyState;

        /*
         * @brief object containing the key map
         */
        std::shared_ptr<KeyTable> m_KeyTable;

        /*!
         * @brief input device we are capturing keys from
         */
        std::shared_ptr<InputHID> m_InputHID;
    };
}  // namespace hemiola
