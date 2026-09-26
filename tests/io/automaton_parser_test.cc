// [Project header — added by Marco]

// File automaton_parser_test.cc: tests for ParseAutomatonFile. Writes
// small definition files under a temporary path, parses them, and
// checks either the resulting PushdownAutomaton or the exception
// thrown for a malformed definition.

#include "include/io/automaton_parser.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "include/exceptions/exceptions.h"

namespace {

// Writes content to a fresh temporary file and returns its path. The
// caller is responsible for removing it (std::remove) once done.
std::string WriteTempFile(const std::string& name, const std::string& content) {
  std::string path = "/tmp/pda_test_" + name + ".txt";
  std::ofstream file(path);
  file << content;
  file.close();
  return path;
}

const char kApvContent[] =
    "# Automata de Pila por vaciado de pila para reconocer el lenguaje\n"
    "#  L = {a^nb^n | n > 0}\n"
    "# Se representa el epsilon con un .\n"
    "q1 q2\n"
    "a b\n"
    "S A\n"
    "q1\n"
    "S\n"
    "q1 a S q1 A\n"
    "q1 a A q1 AA\n"
    "q1 b A q2 .\n"
    "q2 b A q2 .\n";

const char kApfContent[] =
    "# Automata de Pila con estados finales para reconocer el lenguaje\n"
    "#  L = {a^nb^n | n > 0}\n"
    "# Se representa el epsilon con un .\n"
    "q1 q2 q3\n"
    "a b\n"
    "S A\n"
    "q1\n"
    "S\n"
    "q3\n"
    "q1 a S q1 AS\n"
    "q1 a A q1 AA\n"
    "q1 b A q2 .\n"
    "q2 b A q2 .\n"
    "q2 . S q3 S\n";

}  // namespace

TEST(AutomatonParserTest, ParsesTheEmptyStackExample) {
  std::string path = WriteTempFile("apv", kApvContent);

  PushdownAutomaton automaton = ParseAutomatonFile(path);

  EXPECT_EQ(automaton.Type(), AutomatonType::kEmptyStack);
  EXPECT_EQ(automaton.Acceptance().Name(), "empty stack");
  EXPECT_EQ(automaton.States().size(), 2u);
  EXPECT_EQ(automaton.Transitions().Size(), 4u);
  EXPECT_EQ(automaton.InitialState(), "q1");
  EXPECT_EQ(automaton.InitialStackSymbol(), 'S');

  std::remove(path.c_str());
}

TEST(AutomatonParserTest, ParsesTheFinalStateExample) {
  std::string path = WriteTempFile("apf", kApfContent);

  PushdownAutomaton automaton = ParseAutomatonFile(path);

  EXPECT_EQ(automaton.Type(), AutomatonType::kFinalState);
  EXPECT_EQ(automaton.Acceptance().Name(), "final state");
  EXPECT_EQ(automaton.States().size(), 3u);
  EXPECT_EQ(automaton.Transitions().Size(), 5u);

  std::remove(path.c_str());
}

TEST(AutomatonParserTest, CommentsAndBlankLinesAreIgnored) {
  std::string content =
      "q1\n\n# a comment\n\na\nZ\nq1\nZ\nq1 a Z q1 Z\n";
  std::string path = WriteTempFile("comments", content);

  PushdownAutomaton automaton = ParseAutomatonFile(path);

  EXPECT_EQ(automaton.Transitions().Size(), 1u);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest, MissingFileThrowsFileNotFoundException) {
  EXPECT_THROW(ParseAutomatonFile("/does/not/exist.txt"),
               FileNotFoundException);
}

TEST(AutomatonParserTest, EmptyFileThrowsEmptyFileException) {
  std::string path = WriteTempFile("empty", "# only a comment\n");
  EXPECT_THROW(ParseAutomatonFile(path), EmptyFileException);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest, TooFewLinesThrowsInvalidDefinitionException) {
  std::string path = WriteTempFile("short", "q1\na\n");
  EXPECT_THROW(ParseAutomatonFile(path), InvalidDefinitionException);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest, InitialStateNotInQThrowsInvalidDefinitionException) {
  std::string content = "q1\na\nZ\nqX\nZ\nq1 a Z q1 Z\n";
  std::string path = WriteTempFile("bad_initial_state", content);

  EXPECT_THROW(ParseAutomatonFile(path), InvalidDefinitionException);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest,
     InitialStackSymbolNotInGammaThrowsInvalidDefinitionException) {
  std::string content = "q1\na\nZ\nq1\nY\nq1 a Z q1 Z\n";
  std::string path = WriteTempFile("bad_initial_stack", content);

  EXPECT_THROW(ParseAutomatonFile(path), InvalidDefinitionException);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest, ReservedCharacterInAlphabetThrowsInvalidDefinition) {
  std::string content = "q1\na .\nZ\nq1\nZ\nq1 a Z q1 Z\n";
  std::string path = WriteTempFile("reserved_symbol", content);

  EXPECT_THROW(ParseAutomatonFile(path), InvalidDefinitionException);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest, TransitionWithWrongFieldCountThrows) {
  std::string content = "q1\na\nZ\nq1\nZ\nq1 a Z q1\n";
  std::string path = WriteTempFile("bad_transition", content);

  EXPECT_THROW(ParseAutomatonFile(path), InvalidDefinitionException);
  std::remove(path.c_str());
}

TEST(AutomatonParserTest, FinalStateNotInQThrowsInvalidDefinitionException) {
  std::string content = "q1 q2\na\nZ\nq1\nZ\nqX\nq1 a Z q1 Z\n";
  std::string path = WriteTempFile("bad_final_state", content);

  EXPECT_THROW(ParseAutomatonFile(path), InvalidDefinitionException);
  std::remove(path.c_str());
}

// A duplicate transition is not an error: it is reported as a warning
// (checked here through stderr capture) and simply not added twice.
TEST(AutomatonParserTest, DuplicateTransitionIsWarnedAboutAndSkipped) {
  std::string content = "q1\na\nZ\nq1\nZ\nq1 a Z q1 Z\nq1 a Z q1 Z\n";
  std::string path = WriteTempFile("duplicate", content);

  testing::internal::CaptureStderr();
  PushdownAutomaton automaton = ParseAutomatonFile(path);
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(automaton.Transitions().Size(), 1u);
  EXPECT_NE(warning_output.find("duplicate"), std::string::npos);
  std::remove(path.c_str());
}