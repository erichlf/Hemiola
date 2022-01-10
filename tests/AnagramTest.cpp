/*
  MIT License
  Copyright (c) 2021 Erich L Foster
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
#include "Anagram.h"

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

TEST ( AnagramTest, NoAnagramTest )
{
    hemiola::Anagram anagram;

    auto result { anagram.lookup ( "" ) };

    EXPECT_EQ ( result.size(), 0 );
}

TEST ( AnagramTest, LowerCaseTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( "cat" );
    anagram.insert ( "act" );

    auto result = anagram.lookup ( "tac" );
    EXPECT_EQ ( result.size(), 2 );
}

TEST ( AnagramTest, UpperCaseTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( "Cat" );
    anagram.insert ( "act" );

    auto result = anagram.lookup ( "tac" );
    EXPECT_EQ ( result.size(), 2 );
}

TEST ( AnagramTest, RepeatedLetterTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( "aa" );
    anagram.insert ( "a" );

    auto result = anagram.lookup ( "AA" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "aa" );

    result = anagram.lookup ( "A" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "a" );
}

TEST ( AnagramTest, SubwordTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( "actor" );
    anagram.insert ( "cat" );

    auto result = anagram.lookup ( "act" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "cat" );
}

TEST ( AnagramTest, NonEnglishCharactersTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( "actor" );
    anagram.insert ( "cät" );

    auto result = anagram.lookup ( "act" );
    EXPECT_EQ ( result.size(), 0 );

    result = anagram.lookup ( "äct" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "cät" );
}
