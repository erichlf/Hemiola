#include <gtest/gtest.h>

#include "Anagram.h"

#include <string>
#include <vector>

TEST ( AnagramTest, NoAnagramTest )
{
    hemiola::Anagram anagram;

    const auto result { anagram.lookup ( "" ) };

    EXPECT_EQ ( result.size(), 0 );
}
