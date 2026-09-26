// File simulator_test.cc: tests for Simulator, built directly on small
// hand-made automata (not through the parser, to keep each test
// focused on the search itself).

#include "include/simulation/simulator.h"

#include <gtest/gtest.h>

#include <sstream>

#include "include/exceptions/exceptions.h"

namespace {

PushdownAutomaton MakeEmptyStackAnBn() {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  input_alphabet.AddSymbol('b');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('S');
  stack_alphabet.AddSymbol('A');
  TransitionTable transitions;
  transitions.Add({"q1", 'a', 'S', "q1", "A"});
  transitions.Add({"q1", 'a', 'A', "q1", "AA"});
  transitions.Add({"q1", 'b', 'A', "q2", ""});
  transitions.Add({"q2", 'b', 'A', "q2", ""});
  return PushdownAutomaton({"q1", "q2"}, input_alphabet, stack_alphabet, "q1",
                            'S', transitions, AutomatonType::kEmptyStack, {});
}

PushdownAutomaton MakeFinalStateAnBn() {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  input_alphabet.AddSymbol('b');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('S');
  stack_alphabet.AddSymbol('A');
  TransitionTable transitions;
  transitions.Add({"q1", 'a', 'S', "q1", "AS"});
  transitions.Add({"q1", 'a', 'A', "q1", "AA"});
  transitions.Add({"q1", 'b', 'A', "q2", ""});
  transitions.Add({"q2", 'b', 'A', "q2", ""});
  transitions.Add({"q2", kEpsilon, 'S', "q3", "S"});
  return PushdownAutomaton({"q1", "q2", "q3"}, input_alphabet, stack_alphabet,
                            "q1", 'S', transitions,
                            AutomatonType::kFinalState, {"q3"});
}

}  // namespace

TEST(SimulatorTest, EmptyStackAcceptsWordsOfTheLanguage) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  Simulator simulator(automaton);

  EXPECT_TRUE(simulator.Accepts("ab"));
  EXPECT_TRUE(simulator.Accepts("aabb"));
  EXPECT_TRUE(simulator.Accepts("aaabbb"));
}

TEST(SimulatorTest, EmptyStackRejectsWordsOutsideTheLanguage) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  Simulator simulator(automaton);

  EXPECT_FALSE(simulator.Accepts(""));   // n > 0, so epsilon is excluded
  EXPECT_FALSE(simulator.Accepts("aab"));
  EXPECT_FALSE(simulator.Accepts("abb"));
  EXPECT_FALSE(simulator.Accepts("ba"));
}

TEST(SimulatorTest, FinalStateAcceptsAndRejectsTheSameWords) {
  PushdownAutomaton automaton = MakeFinalStateAnBn();
  Simulator simulator(automaton);

  EXPECT_TRUE(simulator.Accepts("ab"));
  EXPECT_TRUE(simulator.Accepts("aabb"));
  EXPECT_FALSE(simulator.Accepts(""));
  EXPECT_FALSE(simulator.Accepts("aab"));
}

TEST(SimulatorTest, SymbolOutsideSigmaThrowsInvalidWordException) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  Simulator simulator(automaton);

  EXPECT_THROW(simulator.Accepts("aXb"), InvalidWordException);
}

TEST(SimulatorTest, TraceWriterReceivesHeaderAndFinalResult) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream trace_out;
  TraceWriter writer(trace_out);
  Simulator simulator(automaton, 500, &writer);

  bool accepted = simulator.Accepts("ab");

  ASSERT_TRUE(accepted);
  std::string trace = trace_out.str();
  EXPECT_NE(trace.find("Word: ab"), std::string::npos);
  EXPECT_NE(trace.find("Result: ACCEPTED"), std::string::npos);
}

TEST(SimulatorTest, ExceedingMaxDepthRejectsTheWord) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  Simulator simulator(automaton, /*max_depth=*/0);

  EXPECT_FALSE(simulator.Accepts("aabb"));
}