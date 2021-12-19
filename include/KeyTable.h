#pragma once

/* a lot of this code was stolen from https://github.com/kernc/logkeys but then modified */

#include <cstring>
#include <string>
#include <vector>

#include <linux/input.h>

namespace hemiola
{
    // forward declaration
    struct KeyState;

    /*!
     * @brief class which contains the system key map
     */
    class KeyTable
    {
    public:
        KeyTable() = default;
        ~KeyTable() = default;

        /*!
         * @brief Update our keymaps based on the system keymapping
         * @post keymaps are updated to coincide with the system keymap
         */
        void determineSystemKeymap();

        /*!
         * @brief check if code is a char
         * @param code the scan code from key press
         * @return true if code is a char
         */
        bool isCharKey ( unsigned int code ) const;

        /*!
         * @brief check if code is a function key
         * @param code the scan code from key press
         * @return true if code is a function key
         */
        bool isFuncKey ( unsigned int code ) const;

        /*!
         * @brief check if code is used
         * @param code the scan code from key press
         * @return true if code is used
         */
        inline bool isUsedKey ( unsigned int code ) const;

        /*!
         * @brief convert a scan code to a character
         * @param code the scan code from key press
         * @param keyState the current state of the keypress
         * @return character representation of key press
         */
        wchar_t handleScanCode ( unsigned int code, const KeyState& keyState ) const;

        /*
         * @brief check if the scan code is a valid scan code
         * @param code the scan code from key press
         * @return true if the code is valid
         */
        bool isCodeValid ( unsigned int code ) const { return code < sizeof ( m_CharOrFunc ); }

    private:
        /*!
         * @brief get the lower case character corresponding to a scan code
         * @param code the scan code from key press
         * @return character representing the scan code
         */
        wchar_t charKeys ( unsigned int code ) const
        {
            return m_CharKeys [toCharKeysIndex ( code )];
        }

        /*!
         * @brief get the upper character corresponding to a scan code
         * @param code the scan code from key press
         * @return character representing the scan code
         */
        wchar_t shiftKeys ( unsigned int code ) const
        {
            return m_ShiftKeys [toCharKeysIndex ( code )];
        }

        /*!
         * @brief get the altgr character corresponding to a scan code
         * @param code the scan code from key press
         * @return character representing the scan code
         */
        wchar_t altgrKeys ( unsigned int code ) const
        {
            return m_AltgrKeys [toCharKeysIndex ( code )];
        }

        /*!
         * @brief get the function key character corresponding to a scan code
         * @param code the scan code from key press
         * @return function key character representing the scan code
         */
        std::wstring funcKeys ( unsigned int code ) const
        {
            return m_FuncKeys [toFuncKeysIndex ( code )];
        }

        /*!
         * @brief translates character keycodes to continuous array indexes
         * @param code the scan code from key press
         * @return the index of character from the keymap
         */
        int toCharKeysIndex ( unsigned int keycode ) const;

        /*!
         * @brief translates function keys keycodes to continuous array indexes
         * @param code the scan code from key press
         * @return the index of function key from the keymap
         */
        int toFuncKeysIndex ( unsigned int keycode ) const;

        // these are ordered default US keymap keys
        /*!
         * @brief US lower case key characters
         */
        std::wstring m_CharKeys { L"1234567890-=qwertyuiop[]asdfghjkl;'`\\zxcvbnm,./<" };
        /*!
         * @brief US upper case key characters
         */
        std::wstring m_ShiftKeys { L"!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\"~|ZXCVBNM<>?>" };
        // TODO: add altgrShiftKeys[]
        // (http://en.wikipedia.org/wiki/AltGr_key#US_international)
        std::wstring m_AltgrKeys { 49, 0 };

        /*!
         * @brief Function Keys
         */
        const std::vector<std::wstring> m_FuncKeys
            = { L"<Esc>",          L"<BckSp>", L"<Tab>",   L"<Enter>",
                L"<LCtrl>",        L"<LShft>", L"<RShft>", L"<KP*>",
                L"<LAlt>",         L" ",       L"<CpsLk>", L"<F1>",
                L"<F2>",           L"<F3>",    L"<F4>",    L"<F5>",
                L"<F6>",           L"<F7>",    L"<F8>",    L"<F9>",
                L"<F10>",          L"<NumLk>", L"<ScrLk>", L"<KP7>",
                L"<KP8>",          L"<KP9>",   L"<KP->",   L"<KP4>",
                L"<KP5>",          L"<KP6>",   L"<KP+>",   L"<KP1>",
                L"<KP2>",          L"<KP3>",   L"<KP0>",   L"<KP.>",
                /*"<",*/ L"<F11>", L"<F12>",   L"<KPEnt>", L"<RCtrl>",
                L"<KP/>",          L"<PrtSc>", L"<AltGr>", L"<Break>" /*linefeed?*/,
                L"<Home>",         L"<Up>",    L"<PgUp>",  L"<Left>",
                L"<Right>",        L"<End>",   L"<Down>",  L"<PgDn>",
                L"<Ins>",          L"<Del>",   L"<Pause>", L"<LMeta>",
                L"<RMeta>",        L"<Menu>" };

        const std::string m_CharOrFunc
            =  // c = character key, f = function key, _ = blank/error ('_' is used, don't change);
               // all according to KEY_* defines from <linux/input.h>
            "_fccccccccccccff"
            "ccccccccccccffcc"
            "ccccccccccfccccc"
            "ccccccffffffffff"
            "ffffffffffffffff"
            "ffff__cff_______"
            "ffffffffffffffff"
            "_______f_____fff";

        unsigned short N_KEYS_DEFINED = 106;  // sum of all 'c' and 'f' chars in charOrFunc[]
    };

}  // namespace hemiola
