#include <gtest/gtest.h>

#include "Anagram.h"

#include <iostream>

#include <string>
#include <vector>

TEST ( AnagramTest, NoAnagramTest )
{
    hemiola::Anagram anagram;

    auto result { anagram.lookup ( std::wstring{} ) };

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
