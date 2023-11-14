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
#include <unordered_map>
#include <vector>

class TestOutputHID : public hemiola::OutputHID
{
public:
    TestOutputHID() = default;
    TestOutputHID ( const TestOutputHID& ) = delete;
    TestOutputHID ( TestOutputHID&& ) = delete;
    TestOutputHID& operator= ( const TestOutputHID& ) = delete;
    TestOutputHID& operator= ( TestOutputHID&& ) = delete;

    ~TestOutputHID() = default;

    void open() override
    { /* no opt */
    }

    void write ( const hemiola::KeyReport& /*report*/ ) const override
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

    const std::unordered_map<unsigned int, hemiola::Hemiola::TimePoint>& captured()
    {
        return m_Hemiola->captured();
    }

private:
    std::shared_ptr<hemiola::KeyTable> m_KeyTable;
    std::shared_ptr<hemiola::KeyChords> m_KeyChords;
    std::shared_ptr<TestOutputHID> m_Output;
    std::shared_ptr<hemiola::Hemiola> m_Hemiola;
};

TEST_F ( HemiolaTest, addKeyTest )
{
    EXPECT_EQ ( this->captured().empty(), true );

    this->addKey ( KEY_H );
    this->addKey ( KEY_E );
    this->addKey ( KEY_M );
    this->addKey ( KEY_I );
    this->addKey ( KEY_O );
    this->addKey ( KEY_L );
    this->addKey ( KEY_A );
    EXPECT_EQ ( this->captured().size(), 7u );
    EXPECT_EQ ( this->captured().count ( KEY_H ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_E ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_M ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_I ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_O ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_L ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_A ), 1u );

    this->addKey ( KEY_SPACE );
    EXPECT_EQ ( this->captured().empty(), true );

    this->addKey ( KEY_RIGHTCTRL );
    this->addKey ( KEY_C );
    this->addKey ( KEY_RIGHTCTRL );
    EXPECT_EQ ( this->captured().empty(), true );

    this->addKey ( KEY_A );
    EXPECT_EQ ( this->captured().size(), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_A ), 1u );
}

TEST_F ( HemiolaTest, backspaceKeyTest )
{
    this->addKey ( KEY_RIGHTSHIFT );
    this->addKey ( KEY_A );
    this->addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( this->captured().size(), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_A ), 1u );

    this->addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( this->captured().empty(), true );

    this->addKey ( KEY_RIGHTSHIFT );
    this->addKey ( KEY_A );
    this->addKey ( KEY_B );
    this->addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( this->captured().size(), 2u );
    EXPECT_EQ ( this->captured().count ( KEY_A ), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_B ), 1u );

    this->addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( this->captured().size(), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_A ), 1u );

    this->addKey ( KEY_RIGHTSHIFT );
    this->addKey ( KEY_RIGHTALT );
    this->addKey ( KEY_A );
    this->addKey ( KEY_RIGHTALT );
    this->addKey ( KEY_RIGHTSHIFT );
    EXPECT_EQ ( this->captured().size(), 1u );
    EXPECT_EQ ( this->captured().count ( KEY_A ), 1u );

    this->addKey ( KEY_BACKSPACE );
    EXPECT_EQ ( this->captured().empty(), true );
}

// TODO: add mock test to be sure other functionality is working like calls to
// OutputHID