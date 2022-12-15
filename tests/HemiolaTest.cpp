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
        for ( const auto& [key, value] : cap_map ) {
            joined = keyTable.charKeys ( key ) + joined;
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

class HemiolaTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_KeyTable = std::make_shared<hemiola::KeyTable>();
        m_KeyChords = std::make_shared<hemiola::KeyChords> ( m_KeyTable );
        m_Output = std::make_shared<TestOutputHID>();

        m_Hemiola = std::make_shared<hemiola::Hemiola> ( m_KeyTable, m_KeyChords, m_Output );
    }

public:
    void addKey ( unsigned int key ) { m_Hemiola->addKey ( key ); }

    std::string captured() { return join ( m_Hemiola->captured() ); }

private:
    std::shared_ptr<hemiola::KeyTable> m_KeyTable;
    std::shared_ptr<hemiola::KeyChords> m_KeyChords;
    std::shared_ptr<TestOutputHID> m_Output;
    std::shared_ptr<hemiola::Hemiola> m_Hemiola;
};

TEST_F ( HemiolaTest, addKeyTest )
{
    using namespace std::string_literals;

    EXPECT_EQ ( this->captured().empty(), true );

    this->addKey ( KEY_H );
    this->addKey ( KEY_E );
    this->addKey ( KEY_M );
    this->addKey ( KEY_I );
    this->addKey ( KEY_O );
    this->addKey ( KEY_L );
    this->addKey ( KEY_A );
    EXPECT_EQ ( this->captured(), "hemiola"s );
    this->addKey ( KEY_SPACE );
    EXPECT_EQ ( this->captured(), ""s );

    this->addKey ( KEY_RIGHTCTRL );
    this->addKey ( KEY_C );
    this->addKey ( KEY_RIGHTCTRL );
    EXPECT_EQ ( this->captured(), ""s );

    this->addKey ( KEY_A );
    EXPECT_EQ ( this->captured(), "a"s );
}

TEST_F ( HemiolaTest, backspaceKeyTest )
{
    using namespace std::string_literals;

    this->addKey ( KEY_RIGHTSHIFT );
    this->addKey ( KEY_A );
    this->addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( this->captured(), "a"s );

    this->addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( this->captured(), ""s );

    this->addKey ( KEY_RIGHTSHIFT );
    this->addKey ( KEY_A );
    this->addKey ( KEY_B );
    this->addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( this->captured(), "ab"s );

    this->addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( this->captured(), "a"s );

    this->addKey ( KEY_RIGHTSHIFT );
    this->addKey ( KEY_RIGHTALT );
    this->addKey ( KEY_A );
    this->addKey ( KEY_RIGHTALT );
    this->addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( this->captured(), "a"s );

    this->addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( this->captured(), ""s );
}
