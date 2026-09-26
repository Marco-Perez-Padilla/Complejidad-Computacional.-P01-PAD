// [Project header — added by Marco]

// File final_state_acceptance_test.cc: tests for the
// acceptance-by-final-state strategy.

#include "acceptance/final_state_acceptance.h"

#include <gtest/gtest.h>

TEST(FinalStateAcceptanceTest, AcceptsWhenInputEmptyAndStateIsFinal) {
  FinalStateAcceptance acceptance({"q2", "q3"});
  Configuration configuration{"q3", "", "Z"};

  EXPECT_TRUE(acceptance.IsAccepting(configuration));
}

TEST(FinalStateAcceptanceTest, RejectsWhenStateIsNotFinal) {
  FinalStateAcceptance acceptance({"q2", "q3"});
  Configuration configuration{"q1", "", "Z"};

  EXPECT_FALSE(acceptance.IsAccepting(configuration));
}

TEST(FinalStateAcceptanceTest, RejectsWhenInputIsNotFullyConsumed) {
  FinalStateAcceptance acceptance({"q2"});
  Configuration configuration{"q2", "b", "Z"};

  EXPECT_FALSE(acceptance.IsAccepting(configuration));
}

// The stack content must be irrelevant for this acceptance mode.
TEST(FinalStateAcceptanceTest, StackContentDoesNotAffectAcceptance) {
  FinalStateAcceptance acceptance({"q2"});
  Configuration configuration{"q2", "", "ZZZ"};

  EXPECT_TRUE(acceptance.IsAccepting(configuration));
}

TEST(FinalStateAcceptanceTest, NameIsFinalState) {
  FinalStateAcceptance acceptance({"q2"});
  EXPECT_EQ(acceptance.Name(), "final state");
}