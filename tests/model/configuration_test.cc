// [Project header — added by Marco]

// File configuration_test.cc: tests for Configuration::Key, used by
// Simulator to detect already-visited configurations.

#include "model/configuration.h"

#include <gtest/gtest.h>

TEST(ConfigurationTest, IdenticalConfigurationsHaveTheSameKey) {
  Configuration a{"q1", "ab", "AZ"};
  Configuration b{"q1", "ab", "AZ"};
  EXPECT_EQ(a.Key(), b.Key());
}

TEST(ConfigurationTest, DifferentStateChangesKey) {
  Configuration a{"q1", "ab", "AZ"};
  Configuration b{"q2", "ab", "AZ"};
  EXPECT_NE(a.Key(), b.Key());
}

TEST(ConfigurationTest, DifferentRemainingInputChangesKey) {
  Configuration a{"q1", "ab", "AZ"};
  Configuration b{"q1", "b", "AZ"};
  EXPECT_NE(a.Key(), b.Key());
}

TEST(ConfigurationTest, DifferentStackChangesKey) {
  Configuration a{"q1", "ab", "AZ"};
  Configuration b{"q1", "ab", "ZZ"};
  EXPECT_NE(a.Key(), b.Key());
}