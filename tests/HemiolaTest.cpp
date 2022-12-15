/*
  MIT License
  Copyright (c) 2021-2022 Erich L Foster
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
#include "KeyChords.h"
#include "KeyTable.h"
#include "OutputHID.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace
{
    /*!
     * @brief Join the elements of a vector into a string
     * @param cap_map A map containing values to be joined
     * @return The values of the map joined as a string
     */
    std::string join ( std::unordered_map<unsigned int, hemiola::Hemiola::TimePoint> cap_map )
    {
        hemiola::KeyTable keyTable;
        std::string joined {};
        for ( const auto & [key, value] : cap_map ) {
            joined += keyTable.charKeys ( key );
        }

        return joined;
    }
}

class TestOutputHID : public hemiola::OutputHID
{
public:
    TestOutputHID() {};
    ~TestOutputHID() = default;

    void open() override
    { /* no opt */
    }
    void write ( const hemiola::KeyReport& /*report*/ ) const
    { /* no opt */
    }
};

TEST ( HemiolaTest, addKeyTest )
{
    using namespace std::string_literals;

    auto keyTable { std::make_shared<hemiola::KeyTable>() };
    auto keyChords { std::make_shared<hemiola::KeyChords> ( keyTable ) };
    auto output { std::make_shared<TestOutputHID>() };

    hemiola::Hemiola hemiola ( keyTable, keyChords, output );

    EXPECT_EQ ( hemiola.captured().empty(), true );

    hemiola.addKey ( KEY_H );
    hemiola.addKey ( KEY_E );
    hemiola.addKey ( KEY_M );
    hemiola.addKey ( KEY_I );
    hemiola.addKey ( KEY_O );
    hemiola.addKey ( KEY_L );
    hemiola.addKey ( KEY_A );
    EXPECT_EQ ( join ( hemiola.captured() ), "hemiola"s );
    hemiola.addKey ( KEY_SPACE );
    EXPECT_EQ ( join ( hemiola.captured() ), ""s );

    hemiola.addKey ( KEY_RIGHTCTRL );
    hemiola.addKey ( KEY_C );
    hemiola.addKey ( KEY_RIGHTCTRL );
    EXPECT_EQ ( join ( hemiola.captured() ), ""s );

    hemiola.addKey ( KEY_A );
    EXPECT_EQ ( join ( hemiola.captured() ), "a"s );
}

TEST ( HemiolaTest, backspaceKeyTest )
{
    using namespace std::string_literals;

    auto keyTable { std::make_shared<hemiola::KeyTable>() };
    auto keyChords { std::make_shared<hemiola::KeyChords> ( keyTable ) };
    auto output { std::make_shared<TestOutputHID>() };

    hemiola::Hemiola hemiola ( keyTable, keyChords, output );

    hemiola.addKey ( KEY_RIGHTSHIFT );
    hemiola.addKey ( KEY_A );
    hemiola.addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( join ( hemiola.captured() ), "a"s );

    hemiola.addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( join ( hemiola.captured() ), ""s );

    hemiola.addKey ( KEY_RIGHTSHIFT );
    hemiola.addKey ( KEY_A );
    hemiola.addKey ( KEY_B );
    hemiola.addKey (  KEY_RIGHTSHIFT );
    EXPECT_EQ ( join ( hemiola.captured() ), "ab"s );

    hemiola.addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( join ( hemiola.captured() ), "a"s );

    hemiola.addKey (  KEY_RIGHTSHIFT );
    hemiola.addKey (  KEY_RIGHTALT );
    hemiola.addKey ( KEY_A );
    hemiola.addKey (  KEY_RIGHTALT );
    hemiola.addKey (  KEY_RIGHTSHIFT );
    EXPECT_EQ ( join ( hemiola.captured() ), "a"s );

    hemiola.addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( join ( hemiola.captured() ), "a"s );
}
