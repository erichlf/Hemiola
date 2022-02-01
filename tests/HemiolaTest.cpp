/*
  MIT License
  Copyright (c) 2021 Erich L Foster
  Pission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to pit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this pission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "Hemiola.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace
{
    /*!
     * @brief Join the elements of a vector into a string
     * @param vs Vector containing elements to be joing
     * param sep The separator used to join the elements of the vector
     * @return The vector joined as a string with each element seperated by a seperated
     */
    std::string join ( std::vector<std::string> vs, std::string sep = "" )
    {
        return fmt::format ( "{}", fmt::join ( vs, sep ) );
    }
}

TEST ( HemiolaTest, addKeyTest )
{
    using namespace std::string_literals;

    auto keyTable { std::make_shared<hemiola::KeyTable>() };

    hemiola::Hemiola hemiola ( keyTable );

    EXPECT_EQ ( hemiola.captured().empty(), true );

    hemiola.addKey ( keyTable->charKeys ( KEY_H ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_E ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_M ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_I ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_O ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_L ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_A ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "hemiola"s );
    hemiola.addKey ( keyTable->charKeys ( KEY_SPACE ) );
    EXPECT_EQ ( join ( hemiola.captured() ), ""s );

    hemiola.addKey ( keyTable->beginModKey ( KEY_RIGHTCTRL ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_C ) );
    hemiola.addKey ( keyTable->endModKey ( KEY_RIGHTCTRL ) );
    EXPECT_EQ ( join ( hemiola.captured() ), ""s );

    hemiola.addKey ( keyTable->charKeys ( KEY_A ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "a"s );
}

TEST ( HemiolaTest, backspaceKeyTest )
{
    using namespace std::string_literals;

    auto keyTable { std::make_shared<hemiola::KeyTable>() };

    hemiola::Hemiola hemiola ( keyTable );

    hemiola.addKey ( keyTable->beginModKey ( KEY_RIGHTSHIFT ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_A ) );
    hemiola.addKey ( keyTable->endModKey ( KEY_RIGHTSHIFT ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "<RSHIFT>a</RSHIFT>"s );

    hemiola.addKey ( keyTable->charKeys ( KEY_BACKSPACE ) );
    EXPECT_EQ ( join ( hemiola.captured() ), ""s );

    hemiola.addKey ( keyTable->beginModKey ( KEY_RIGHTSHIFT ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_A ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_B ) );
    hemiola.addKey ( keyTable->endModKey ( KEY_RIGHTSHIFT ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "<RSHIFT>ab</RSHIFT>"s );

    hemiola.addKey ( keyTable->charKeys ( KEY_BACKSPACE ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "<RSHIFT>a</RSHIFT>"s );

    hemiola.addKey ( keyTable->beginModKey ( KEY_RIGHTSHIFT ) );
    hemiola.addKey ( keyTable->beginModKey ( KEY_RIGHTALT ) );
    hemiola.addKey ( keyTable->charKeys ( KEY_A ) );
    hemiola.addKey ( keyTable->endModKey ( KEY_RIGHTALT ) );
    hemiola.addKey ( keyTable->endModKey ( KEY_RIGHTSHIFT ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "<RSHIFT>a</RSHIFT><RSHIFT><RALT>a</RALT></RSHIFT>"s );

    hemiola.addKey ( keyTable->charKeys ( KEY_BACKSPACE ) );
    EXPECT_EQ ( join ( hemiola.captured() ), "<RSHIFT>a</RSHIFT>"s );
}
