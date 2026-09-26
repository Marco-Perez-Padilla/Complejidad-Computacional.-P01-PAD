// File acceptance_factory_test.cc: tests for MakeAcceptanceCriterion.
// The concrete strategy is checked indirectly, through its Name() and
// IsAccepting() behavior, since the factory only returns the abstract
// interface.

#include "acceptance/acceptance_factory.h"

#include <gtest/gtest.h>

TEST(AcceptanceFactoryTest, KFinalStateProducesFinalStateAcceptance) {
  std::unique_ptr<AcceptanceCriterion> acceptance =
      MakeAcceptanceCriterion(AutomatonType::kFinalState, {"q2"});

  ASSERT_NE(acceptance, nullptr);
  EXPECT_EQ(acceptance->Name(), "final state");
  EXPECT_TRUE(acceptance->IsAccepting(Configuration{"q2", "", "ZZ"}));
  EXPECT_FALSE(acceptance->IsAccepting(Configuration{"q1", "", ""}));
}

TEST(AcceptanceFactoryTest, KEmptyStackProducesEmptyStackAcceptance) {
  std::unique_ptr<AcceptanceCriterion> acceptance =
      MakeAcceptanceCriterion(AutomatonType::kEmptyStack, {});

  ASSERT_NE(acceptance, nullptr);
  EXPECT_EQ(acceptance->Name(), "empty stack");
  EXPECT_TRUE(acceptance->IsAccepting(Configuration{"q1", "", ""}));
  EXPECT_FALSE(acceptance->IsAccepting(Configuration{"q1", "", "Z"}));
}