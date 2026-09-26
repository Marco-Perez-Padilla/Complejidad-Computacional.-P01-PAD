// [Project header — added by Marco]

// File menu_test.cc: tests for Menu, driven entirely through
// istringstream/ostringstream instead of std::cin/std::cout.

#include "include/io/menu.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>

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

}  // namespace

TEST(MenuTest, KeyboardModeChecksEachWordUntilDoubleDot) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::istringstream in("1\naabb\naab\n..\n3\n");
  Menu menu(checker, in, out);

  menu.Run();

  std::string output = out.str();
  EXPECT_NE(output.find("'aabb': ACCEPTED"), std::string::npos);
  EXPECT_NE(output.find("'aab': REJECTED"), std::string::npos);
}

TEST(MenuTest, KeyboardModeTreatsDotAsTheEmptyWord) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::istringstream in("1\n.\n..\n3\n");
  Menu menu(checker, in, out);

  menu.Run();

  EXPECT_NE(out.str().find("'eps': REJECTED"), std::string::npos);
}

TEST(MenuTest, FileModeChecksWordsFromTheGivenFile) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);

  std::string path = "/tmp/pda_menu_test_words.txt";
  std::ofstream file(path);
  file << "aabb\nab\n";
  file.close();

  std::istringstream in("2\n" + path + "\n3\n");
  Menu menu(checker, in, out);

  menu.Run();

  std::string output = out.str();
  EXPECT_NE(output.find("'aabb': ACCEPTED"), std::string::npos);
  EXPECT_NE(output.find("'ab': ACCEPTED"), std::string::npos);
  std::remove(path.c_str());
}

// A missing file in file mode is a non-critical error: it is reported
// as a warning and the menu keeps running.
TEST(MenuTest, FileModeWithMissingFileWarnsAndReturnsToMenu) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::istringstream in("2\n/does/not/exist.txt\n3\n");
  Menu menu(checker, in, out);

  testing::internal::CaptureStderr();
  menu.Run();
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_NE(warning_output.find("exist.txt"), std::string::npos);
}

TEST(MenuTest, InvalidOptionWarnsAndTheMenuKeepsRunning) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::istringstream in("9\n3\n");
  Menu menu(checker, in, out);

  testing::internal::CaptureStderr();
  menu.Run();
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_NE(warning_output.find("invalid menu option"), std::string::npos);
}

TEST(MenuTest, EndOfInputStopsTheMenuWithoutChoosingExit) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::istringstream in("");  // no input at all, not even a menu choice
  Menu menu(checker, in, out);

  EXPECT_NO_THROW(menu.Run());
}