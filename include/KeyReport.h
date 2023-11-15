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

#include <linux/input.h>

#include <array>

namespace hemiola
{
    using KeyArray = std::array<uint8_t, 6>;
    /*!
     * @brief struct describing the current key press
     */
    struct KeyReport
    {
        /*!
         * @modifier hex codes (bit 0 is L CTRL, bit 1 is L SHIFT, bit 2 is L ALT, bit 3 is L GUI,
         *                      bit 4 is R CTRL, bit 5 is R SHIFT, bit 6 is R ALT, and bit 7 is R
         *                      GUI).
         */
        uint8_t modifiers;
        /*!
         * @brief list of keys pressed with modifier (6 allowed)
         */
        KeyArray keys {};

        /*!
         * @brief Add key to the key report
         * @returns true if the key was added to the report and false otherwise
         */
        bool setKey ( const uint8_t scanHex )
        {
            // add current key press to the list of key presses, and don't overwrite
            // TODO: handle KEY_ERR_OVF
            bool overflow = true;
            for ( auto& code : keys ) {
                // key was already added to report
                if ( code == scanHex ) {
                    overflow = false;
                    break;
                } else if ( code == 0x00 ) {
                    code = scanHex;
                    overflow = false;
                    break;
                }
            }
            return overflow;
        }

        /*!
         * @brief If a scancode exists in the key report remove it
         */
        void unsetKey ( const uint8_t scanHex )
        {
            // find the key and set to 0
            for ( auto& code : keys ) {
                if ( code == scanHex ) {
                    code = 0x00;
                    break;
                }
            }
        }

        void setModifier ( const uint8_t scanHex ) { modifiers |= scanHex; }

        void unsetModifier ( const uint8_t scanHex ) { modifiers &= ~scanHex; }
    };

    /*!
     * @brief comparison operator for KeyReport
     */
    inline bool operator== ( const KeyReport& lhs, const KeyReport& rhs )
    {
        return lhs.modifiers == rhs.modifiers
               && ( lhs.keys [0] == rhs.keys [0] && lhs.keys [1] == rhs.keys [1]
                    && lhs.keys [2] == rhs.keys [2] && lhs.keys [3] == rhs.keys [3]
                    && lhs.keys [4] == rhs.keys [4] && lhs.keys [5] == rhs.keys [5] );
    }
}
