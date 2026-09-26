// File trace_writer_test.cc: tests for TraceWriter. Each Print*
// method is checked against an in-memory ostringstream, so no real
// file or terminal is needed.

#include "include/io/trace_writer.h"

#include <gtest/gtest.h>

#include <sstream>

TEST(TraceWriterTest, PrintHeaderShowsWordAndAcceptanceMode) {
  std::ostringstream out;
  TraceWriter writer(out);

  writer.PrintHeader("aabb", "final state");

  EXPECT_EQ(out.str(), "Word: aabb  [acceptance by final state]\n");
}

TEST(TraceWriterTest, PrintHeaderShowsEpsForTheEmptyWord) {
  std::ostringstream out;
  TraceWriter writer(out);

  writer.PrintHeader("", "empty stack");

  EXPECT_NE(out.str().find("eps"), std::string::npos);
}

TEST(TraceWriterTest, PrintConfigurationShowsStateInputAndStack) {
  std::ostringstream out;
  TraceWriter writer(out);
  Configuration configuration{"q1", "ab", "AZ"};

  writer.PrintConfiguration(0, configuration, {});

  std::string output = out.str();
  EXPECT_NE(output.find("q1"), std::string::npos);
  EXPECT_NE(output.find("ab"), std::string::npos);
  EXPECT_NE(output.find("AZ"), std::string::npos);
}

TEST(TraceWriterTest, PrintConfigurationReportsNumberOfApplicableTransitions) {
  std::ostringstream out;
  TraceWriter writer(out);
  Configuration configuration{"q1", "a", "Z"};
  std::vector<Transition> applicable{{"q1", 'a', 'Z', "q1", "AZ"}};

  writer.PrintConfiguration(0, configuration, applicable);

  EXPECT_NE(out.str().find("applicable: 1"), std::string::npos);
}

TEST(TraceWriterTest, PrintConfigurationReportsNoApplicableTransitions) {
  std::ostringstream out;
  TraceWriter writer(out);
  Configuration configuration{"q1", "a", "Z"};

  writer.PrintConfiguration(0, configuration, {});

  EXPECT_NE(out.str().find("no applicable transitions"), std::string::npos);
}

TEST(TraceWriterTest, DeeperConfigurationsAreIndentedFurther) {
  std::ostringstream shallow;
  std::ostringstream deep;
  TraceWriter shallow_writer(shallow);
  TraceWriter deep_writer(deep);
  Configuration configuration{"q1", "a", "Z"};

  shallow_writer.PrintConfiguration(0, configuration, {});
  deep_writer.PrintConfiguration(2, configuration, {});

  EXPECT_LT(shallow.str().find('('), deep.str().find('('));
}

TEST(TraceWriterTest, PrintTransitionShowsFromAndToTuples) {
  std::ostringstream out;
  TraceWriter writer(out);
  Transition transition{"q1", 'a', 'Z', "q2", "AZ"};

  writer.PrintTransition(0, transition);

  std::string output = out.str();
  EXPECT_NE(output.find("q1"), std::string::npos);
  EXPECT_NE(output.find("q2"), std::string::npos);
  EXPECT_NE(output.find("AZ"), std::string::npos);
}

TEST(TraceWriterTest, PrintTransitionShowsEpsForAnEpsilonInputOrEmptyPush) {
  std::ostringstream out;
  TraceWriter writer(out);
  Transition transition{"q1", kEpsilon, 'Z', "q2", ""};

  writer.PrintTransition(0, transition);

  std::string output = out.str();
  EXPECT_NE(output.find("eps"), std::string::npos);
}

TEST(TraceWriterTest, PrintAcceptedWritesAccepted) {
  std::ostringstream out;
  TraceWriter writer(out);

  writer.PrintAccepted(1);

  EXPECT_NE(out.str().find("ACCEPTED"), std::string::npos);
}

TEST(TraceWriterTest, PrintDeadEndWritesDeadEnd) {
  std::ostringstream out;
  TraceWriter writer(out);

  writer.PrintDeadEnd(1);

  EXPECT_NE(out.str().find("dead end"), std::string::npos);
}

TEST(TraceWriterTest, PrintResultDistinguishesAcceptedFromRejected) {
  std::ostringstream accepted_out;
  std::ostringstream rejected_out;
  TraceWriter accepted_writer(accepted_out);
  TraceWriter rejected_writer(rejected_out);

  accepted_writer.PrintResult(true);
  rejected_writer.PrintResult(false);

  EXPECT_NE(accepted_out.str().find("ACCEPTED"), std::string::npos);
  EXPECT_NE(rejected_out.str().find("REJECTED"), std::string::npos);
}