#include <gtest/gtest.h>

#include "Anagram.h"

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
    EXPECT_EQ ( result[0], "aa" );

    result = anagram.lookup ( "A" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result[0], "a" );
}

TEST ( AnagramTest, SubwordTest )
{
    hemiola::Anagram anagram;

    anagram.insert ( "actor" );
    anagram.insert ( "cat" );

    auto result = anagram.lookup ( "act" );
    EXPECT_EQ ( result.size(), 1 );
    EXPECT_EQ ( result[0], "cat" );
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
    EXPECT_EQ ( result[0], "cät" );
}
