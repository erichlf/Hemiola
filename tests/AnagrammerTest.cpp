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
#include "Anagrammer.h"

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

TEST ( AnagrammerTest, NoAnagrammerTest )
{
    hemiola::Anagrammer anagrammer;

    auto result { anagrammer.lookup ( "" ) };

    EXPECT_EQ ( result.size(), 0 );
}

TEST ( AnagrammerTest, LowerCaseTest )
{
    hemiola::Anagrammer anagrammer;

    anagrammer.insert ( "cat" );
    anagrammer.insert ( "act" );

    auto result = anagrammer.lookup ( "tac" );
    EXPECT_EQ ( result.size(), 2 );
}

TEST ( AnagrammerTest, UpperCaseTest )
{
    hemiola::Anagrammer anagrammer;

    anagrammer.insert ( "Cat" );
    anagrammer.insert ( "act" );

    auto result = anagrammer.lookup ( "tac" );
    EXPECT_EQ ( result.size(), 2 );
}

TEST ( AnagrammerTest, RepeatedLetterTest )
{
    hemiola::Anagrammer anagrammer;

    anagrammer.insert ( "aa" );
    anagrammer.insert ( "a" );

    auto result = anagrammer.lookup ( "AA" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "aa" );

    result = anagrammer.lookup ( "A" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "a" );
}

TEST ( AnagrammerTest, SubwordTest )
{
    hemiola::Anagrammer anagrammer;

    anagrammer.insert ( "actor" );
    anagrammer.insert ( "cat" );

    auto result = anagrammer.lookup ( "act" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "cat" );
}

TEST ( AnagrammerTest, NonEnglishCharactersTest )
{
    hemiola::Anagrammer anagrammer;

    anagrammer.insert ( "actor" );
    anagrammer.insert ( "cät" );

    auto result = anagrammer.lookup ( "act" );
    EXPECT_EQ ( result.size(), 0 );

    result = anagrammer.lookup ( "äct" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result [0], "cät" );
}
