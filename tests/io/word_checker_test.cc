// File word_checker_test.cc: tests for WordChecker, both for a single
// Check() call and for CheckAllFromFile().

#include "include/io/word_checker.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
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

std::string WriteTempFile(const std::string& name, const std::string& content) {
  std::string path = "/tmp/pda_word_checker_" + name + ".txt";
  std::ofstream file(path);
  file << content;
  file.close();
  return path;
}

}  // namespace

TEST(WordCheckerTest, CheckPrintsAcceptedOrRejected) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, /*trace=*/false, out, trace_out);

  checker.Check("aabb");
  checker.Check("aab");

  std::string output = out.str();
  EXPECT_NE(output.find("'aabb': ACCEPTED"), std::string::npos);
  EXPECT_NE(output.find("'aab': REJECTED"), std::string::npos);
}

TEST(WordCheckerTest, DotIsTreatedAsTheEmptyWord) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);

  checker.Check(".");

  EXPECT_NE(out.str().find("'eps': REJECTED"), std::string::npos);
}

TEST(WordCheckerTest, InvalidSymbolIsWarnedAboutNotThrown) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);

  testing::internal::CaptureStderr();
  EXPECT_NO_THROW(checker.Check("aXb"));
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_NE(warning_output.find("aXb"), std::string::npos);
  EXPECT_TRUE(out.str().empty());
}

TEST(WordCheckerTest, TraceIsWrittenOnlyWhenTraceIsEnabled) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, /*trace=*/true, out, trace_out);

  checker.Check("ab");

  EXPECT_FALSE(trace_out.str().empty());
}

TEST(WordCheckerTest, CheckAllFromFileChecksEveryNonEmptyLine) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::string path = WriteTempFile("words", "aabb\nab\n\naab\n");

  checker.CheckAllFromFile(path);

  std::string output = out.str();
  EXPECT_NE(output.find("'aabb': ACCEPTED"), std::string::npos);
  EXPECT_NE(output.find("'ab': ACCEPTED"), std::string::npos);
  EXPECT_NE(output.find("'aab': REJECTED"), std::string::npos);
  std::remove(path.c_str());
}

TEST(WordCheckerTest, CheckAllFromFileSkipsAnInvalidWordAndContinues) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::string path = WriteTempFile("words_with_error", "aXb\naabb\n");

  testing::internal::CaptureStderr();
  checker.CheckAllFromFile(path);
  testing::internal::GetCapturedStderr();

  EXPECT_NE(out.str().find("'aabb': ACCEPTED"), std::string::npos);
  std::remove(path.c_str());
}

TEST(WordCheckerTest, CheckAllFromFileReportsTheLineOfAnInvalidWord) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);
  std::string path = WriteTempFile("words_line_numbers", "aabb\n\naXb\nc\n");

  testing::internal::CaptureStderr();
  checker.CheckAllFromFile(path);
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_NE(warning_output.find("line 3"), std::string::npos);
  EXPECT_NE(warning_output.find("line 4"), std::string::npos);
  std::remove(path.c_str());
}

TEST(WordCheckerTest, CheckWithoutALineNumberOmitsIt) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);

  testing::internal::CaptureStderr();
  checker.Check("aXb");
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(warning_output.find("line"), std::string::npos);
}

TEST(WordCheckerTest, CheckAllFromFileThrowsWhenTheFileDoesNotExist) {
  PushdownAutomaton automaton = MakeEmptyStackAnBn();
  std::ostringstream out;
  std::ostringstream trace_out;
  WordChecker checker(automaton, false, out, trace_out);

  EXPECT_THROW(checker.CheckAllFromFile("/does/not/exist.txt"),
               FileNotFoundException);
}