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
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "Anagram.h"

TEST ( AnagramTest, NoAnagramTest )
{
    hemiola::Anagram anagram;

    auto result { anagram.lookup ( std::wstring {} ) };

    EXPECT_EQ ( result.size(), 0 );
}

TEST ( AnagramTest, LowerCaseTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( L"cat" );
    anagram.insert ( L"act" );

    auto result = anagram.lookup ( L"tac" );
    EXPECT_EQ ( result.size(), 2 );
}

TEST ( AnagramTest, UpperCaseTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( L"Cat" );
    anagram.insert ( L"act" );

    auto result = anagram.lookup ( L"tac" );
    EXPECT_EQ ( result.size(), 2 );
}

TEST ( AnagramTest, RepeatedLetterTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( L"aa" );
    anagram.insert ( L"a" );

    auto result = anagram.lookup ( L"AA" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result.at ( 0 ), L"aa" );

    result = anagram.lookup ( L"A" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result.at ( 0 ), L"a" );
}

TEST ( AnagramTest, SubwordTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( L"actor" );
    anagram.insert ( L"cat" );

    auto result = anagram.lookup ( L"act" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result.at ( 0 ), L"cat" );
}

TEST ( AnagramTest, NonEnglishCharactersTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( L"actor" );
    anagram.insert ( L"cät" );
    // Ŕ is a wide character, so this is a bit harder of a test
    anagram.insert ( L"Ŕät" );

    auto result = anagram.lookup ( L"act" );
    EXPECT_EQ ( result.size(), 0 );

    result = anagram.lookup ( L"äct" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result.at ( 0 ), L"cät" );

    // sometimes there is no longer case version of a character
    result = anagram.lookup ( L"äŔt" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result.at ( 0 ), L"Ŕät" );
}
