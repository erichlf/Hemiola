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
#include "KeyTable.h"

#include <gtest/gtest.h>

TEST ( KeyTableTest, isKeyValidTest )
{
    hemiola::KeyTable keyTable;

    EXPECT_EQ ( keyTable.isKeyValid ( 0 ), false );
    EXPECT_EQ ( keyTable.isKeyValid ( KEY_BACKSPACE ), true );
    EXPECT_EQ ( keyTable.isKeyValid ( KEY_RIGHTMETA ), true );
}

TEST ( KeyTableTest, isCharKeyTest )
{
    hemiola::KeyTable keyTable;

    EXPECT_EQ ( keyTable.isCharKey ( 0 ), false );
    EXPECT_EQ ( keyTable.isCharKey ( KEY_W ), true );
    EXPECT_EQ ( keyTable.isCharKey ( KEY_8 ), true );
    EXPECT_EQ ( keyTable.isCharKey ( KEY_BACKSPACE ), true );
    EXPECT_EQ ( keyTable.isCharKey ( KEY_LEFTCTRL ), false );
}

TEST ( KeyTableTest, isModifierTest )
{
    hemiola::KeyTable keyTable;

    EXPECT_EQ ( keyTable.isModifier ( KEY_RESERVED ), false );

    EXPECT_EQ ( keyTable.isModifier ( KEY_LEFTCTRL ), true );
    EXPECT_EQ ( keyTable.isModifier ( KEY_RIGHTCTRL ), true );
    EXPECT_EQ ( keyTable.isModifier ( KEY_RIGHTSHIFT ), true );

    EXPECT_EQ ( keyTable.isModifier ( KEY_A ), false );
}
