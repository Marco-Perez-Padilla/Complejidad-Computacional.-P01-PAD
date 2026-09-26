// [Project header — added by Marco]

// File empty_stack_acceptance_test.cc: tests for the
// acceptance-by-empty-stack strategy.

#include "acceptance/empty_stack_acceptance.h"

#include <gtest/gtest.h>

TEST(EmptyStackAcceptanceTest, AcceptsWhenInputAndStackAreEmpty) {
  EmptyStackAcceptance acceptance;
  Configuration configuration{"q2", "", ""};

  EXPECT_TRUE(acceptance.IsAccepting(configuration));
}

TEST(EmptyStackAcceptanceTest, RejectsWhenStackIsNotEmpty) {
  EmptyStackAcceptance acceptance;
  Configuration configuration{"q2", "", "Z"};

  EXPECT_FALSE(acceptance.IsAccepting(configuration));
}

TEST(EmptyStackAcceptanceTest, RejectsWhenInputIsNotFullyConsumed) {
  EmptyStackAcceptance acceptance;
  Configuration configuration{"q2", "b", ""};

  EXPECT_FALSE(acceptance.IsAccepting(configuration));
}

// The current state must be irrelevant for this acceptance mode.
TEST(EmptyStackAcceptanceTest, StateDoesNotAffectAcceptance) {
  EmptyStackAcceptance acceptance;
  Configuration configuration{"any_state", "", ""};

  EXPECT_TRUE(acceptance.IsAccepting(configuration));
}

TEST(EmptyStackAcceptanceTest, NameIsEmptyStack) {
  EmptyStackAcceptance acceptance;
  EXPECT_EQ(acceptance.Name(), "empty stack");
}