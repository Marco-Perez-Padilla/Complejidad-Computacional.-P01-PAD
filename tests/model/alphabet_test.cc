// [Project header — added by Marco]

// File alphabet_test.cc: tests for the Alphabet class.

#include "model/alphabet.h"

#include <gtest/gtest.h>

TEST(AlphabetTest, StartsEmpty) {
  Alphabet alphabet;
  EXPECT_TRUE(alphabet.Empty());
  EXPECT_EQ(alphabet.Size(), 0u);
}

TEST(AlphabetTest, AddSymbolAddsAndReportsMembership) {
  Alphabet alphabet;
  EXPECT_TRUE(alphabet.AddSymbol('a'));
  EXPECT_TRUE(alphabet.Contains('a'));
  EXPECT_FALSE(alphabet.Contains('b'));
  EXPECT_FALSE(alphabet.Empty());
  EXPECT_EQ(alphabet.Size(), 1u);
}

TEST(AlphabetTest, AddingTheSameSymbolTwiceDoesNotGrow) {
  Alphabet alphabet;
  alphabet.AddSymbol('a');
  alphabet.AddSymbol('a');
  EXPECT_EQ(alphabet.Size(), 1u);
}

TEST(AlphabetTest, ReservedSymbolsAreRejected) {
  Alphabet alphabet;
  EXPECT_FALSE(alphabet.AddSymbol('.'));
  EXPECT_FALSE(alphabet.AddSymbol('#'));
  EXPECT_TRUE(alphabet.Empty());
}

TEST(AlphabetTest, IsReservedIdentifiesOnlyDotAndHash) {
  EXPECT_TRUE(Alphabet::IsReserved('.'));
  EXPECT_TRUE(Alphabet::IsReserved('#'));
  EXPECT_FALSE(Alphabet::IsReserved('a'));
  EXPECT_FALSE(Alphabet::IsReserved('Z'));
}