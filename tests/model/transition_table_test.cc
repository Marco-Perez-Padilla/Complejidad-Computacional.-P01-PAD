// File transition_table_test.cc: tests for TransitionTable.

#include "model/transition_table.h"

#include <gtest/gtest.h>

TEST(TransitionTableTest, StartsEmpty) {
  TransitionTable table;
  EXPECT_EQ(table.Size(), 0u);
  EXPECT_TRUE(table.All().empty());
}

TEST(TransitionTableTest, FindReturnsTheMatchingTransition) {
  TransitionTable table;
  table.Add({"q1", 'a', 'Z', "q2", "AZ"});

  std::vector<Transition> matches = table.Find("q1", 'a', 'Z');

  ASSERT_EQ(matches.size(), 1u);
  EXPECT_EQ(matches[0].to, "q2");
  EXPECT_EQ(matches[0].push, "AZ");
}

TEST(TransitionTableTest, FindReturnsEmptyWhenNothingMatches) {
  TransitionTable table;
  table.Add({"q1", 'a', 'Z', "q2", "AZ"});

  EXPECT_TRUE(table.Find("q1", 'b', 'Z').empty());
  EXPECT_TRUE(table.Find("q2", 'a', 'Z').empty());
  EXPECT_TRUE(table.Find("q1", 'a', 'A').empty());
}

TEST(TransitionTableTest, FindReturnsAllNonDeterministicMatches) {
  TransitionTable table;
  table.Add({"q1", 'a', 'Z', "q1", "AZ"});
  table.Add({"q1", 'a', 'Z', "q2", "Z"});

  std::vector<Transition> matches = table.Find("q1", 'a', 'Z');

  ASSERT_EQ(matches.size(), 2u);
  EXPECT_EQ(matches[0].to, "q1");
  EXPECT_EQ(matches[1].to, "q2");
}

TEST(TransitionTableTest, FindMatchesEpsilonTransitionsByTheirOwnMarker) {
  TransitionTable table;
  table.Add({"q1", kEpsilon, 'Z', "q2", "Z"});

  EXPECT_EQ(table.Find("q1", kEpsilon, 'Z').size(), 1u);
  EXPECT_TRUE(table.Find("q1", 'a', 'Z').empty());
}

TEST(TransitionTableTest, AllReturnsEveryTransitionInInsertionOrder) {
  TransitionTable table;
  table.Add({"q1", 'a', 'Z', "q1", "AZ"});
  table.Add({"q1", 'b', 'A', "q2", ""});

  ASSERT_EQ(table.All().size(), 2u);
  EXPECT_EQ(table.All()[0].input, 'a');
  EXPECT_EQ(table.All()[1].input, 'b');
}