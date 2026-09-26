// [Project header — added by Marco]

// File pushdown_automaton_test.cc: tests for PushdownAutomaton,
// covering both its getters and the validation performed by its
// constructor.

#include "model/pushdown_automaton.h"

#include <gtest/gtest.h>

#include "exceptions/exceptions.h"

namespace {

// Builds a minimal well-formed automaton, so every test only has to
// change the one thing it is actually checking.
PushdownAutomaton MakeValidAutomaton() {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"q1", 'a', 'Z', "q1", "Z"});
  return PushdownAutomaton({"q1", "q2"}, input_alphabet, stack_alphabet, "q1",
                            'Z', transitions, AutomatonType::kEmptyStack, {});
}

}  // namespace

TEST(PushdownAutomatonTest, GettersReturnWhatWasPassedIn) {
  PushdownAutomaton automaton = MakeValidAutomaton();

  EXPECT_EQ(automaton.States().size(), 2u);
  EXPECT_TRUE(automaton.InputAlphabet().Contains('a'));
  EXPECT_TRUE(automaton.StackAlphabet().Contains('Z'));
  EXPECT_EQ(automaton.InitialState(), "q1");
  EXPECT_EQ(automaton.InitialStackSymbol(), 'Z');
  EXPECT_EQ(automaton.Transitions().Size(), 1u);
  EXPECT_EQ(automaton.Type(), AutomatonType::kEmptyStack);
  EXPECT_EQ(automaton.Acceptance().Name(), "empty stack");
}

TEST(PushdownAutomatonTest, RejectsEmptyStateSet) {
  Alphabet input_alphabet;
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  EXPECT_THROW(PushdownAutomaton({}, input_alphabet, stack_alphabet, "q1", 'Z',
                                  TransitionTable(),
                                  AutomatonType::kEmptyStack, {}),
               InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsInitialStateNotInQ) {
  Alphabet input_alphabet;
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "qX", 'Z',
                         TransitionTable(), AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsInitialStackSymbolNotInGamma) {
  Alphabet input_alphabet;
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Y',
                         TransitionTable(), AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsFinalStateNotInQ) {
  Alphabet input_alphabet;
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  EXPECT_THROW(PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1",
                                  'Z', TransitionTable(),
                                  AutomatonType::kFinalState, {"qX"}),
               InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsTransitionWithUnknownOriginState) {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"qX", 'a', 'Z', "q1", "Z"});
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Z',
                         transitions, AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsTransitionWithInputOutsideSigma) {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"q1", 'b', 'Z', "q1", "Z"});
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Z',
                         transitions, AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsTransitionWithTopOutsideGamma) {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"q1", 'a', 'Y', "q1", "Z"});
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Z',
                         transitions, AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsTransitionWithUnknownDestinationState) {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"q1", 'a', 'Z', "qX", "Z"});
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Z',
                         transitions, AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

TEST(PushdownAutomatonTest, RejectsTransitionPushingSymbolOutsideGamma) {
  Alphabet input_alphabet;
  input_alphabet.AddSymbol('a');
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"q1", 'a', 'Z', "q1", "ZY"});
  EXPECT_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Z',
                         transitions, AutomatonType::kEmptyStack, {}),
      InvalidDefinitionException);
}

// An epsilon transition (input == kEpsilon) must never be rejected as
// "input outside Sigma", since it is not meant to be in Sigma at all.
TEST(PushdownAutomatonTest, AcceptsEpsilonTransition) {
  Alphabet input_alphabet;
  Alphabet stack_alphabet;
  stack_alphabet.AddSymbol('Z');
  TransitionTable transitions;
  transitions.Add({"q1", kEpsilon, 'Z', "q1", "Z"});
  EXPECT_NO_THROW(
      PushdownAutomaton({"q1"}, input_alphabet, stack_alphabet, "q1", 'Z',
                         transitions, AutomatonType::kEmptyStack, {}));
}