// File application_test.cc: end-to-end tests for Application::Run,
// using real temporary automaton/word files and istringstream/
// ostringstream instead of std::cin/std::cout.

#include "include/io/application.h"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>

#include "include/exceptions/exceptions.h"

namespace {

const char kApvContent[] =
    "q1 q2\n"
    "a b\n"
    "S A\n"
    "q1\n"
    "S\n"
    "q1 a S q1 A\n"
    "q1 a A q1 AA\n"
    "q1 b A q2 .\n"
    "q2 b A q2 .\n";

std::string WriteTempFile(const std::string& name, const std::string& content) {
  std::string path = "/tmp/pda_application_test_" + name + ".txt";
  std::ofstream file(path);
  file << content;
  file.close();
  return path;
}

}  // namespace

TEST(ApplicationTest, RunWithInputFileChecksEveryWordAndPrintsSummary) {
  std::string automaton_path = WriteTempFile("automaton", kApvContent);
  std::string words_path = WriteTempFile("words", "aabb\naab\n");

  Options options;
  options.config_file = automaton_path;
  options.type = AutomatonType::kEmptyStack;
  options.input_file = words_path;

  std::istringstream in;
  std::ostringstream out;
  Application application(options, in, out);

  application.Run();

  std::string output = out.str();
  EXPECT_NE(output.find("Automaton loaded"), std::string::npos);
  EXPECT_NE(output.find("'aabb': ACCEPTED"), std::string::npos);
  EXPECT_NE(output.find("'aab': REJECTED"), std::string::npos);

  std::remove(automaton_path.c_str());
  std::remove(words_path.c_str());
}

TEST(ApplicationTest, RunWithoutInputFileGoesThroughTheInteractiveMenu) {
  std::string automaton_path = WriteTempFile("automaton_menu", kApvContent);

  Options options;
  options.config_file = automaton_path;
  options.type = AutomatonType::kEmptyStack;

  std::istringstream in("1\naabb\n..\n3\n");
  std::ostringstream out;
  Application application(options, in, out);

  application.Run();

  std::string output = out.str();
  EXPECT_NE(output.find("Automaton loaded"), std::string::npos);
  EXPECT_NE(output.find("'aabb': ACCEPTED"), std::string::npos);

  std::remove(automaton_path.c_str());
}

TEST(ApplicationTest, RunWithMissingConfigFileThrowsFileNotFoundException) {
  Options options;
  options.config_file = "/does/not/exist.txt";
  options.type = AutomatonType::kEmptyStack;

  std::istringstream in;
  std::ostringstream out;
  Application application(options, in, out);

  EXPECT_THROW(application.Run(), FileNotFoundException);
}

TEST(ApplicationTest, RunWithTraceAndOutputFileWritesTheTraceToThatFile) {
  std::string automaton_path = WriteTempFile("automaton_trace", kApvContent);
  std::string words_path = WriteTempFile("words_trace", "ab\n");
  std::string trace_path = "/tmp/pda_application_test_trace_out.txt";

  Options options;
  options.config_file = automaton_path;
  options.type = AutomatonType::kEmptyStack;
  options.input_file = words_path;
  options.trace = true;
  options.output_file = trace_path;

  std::istringstream in;
  std::ostringstream out;
  Application application(options, in, out);

  application.Run();

  std::ifstream trace_file(trace_path);
  std::ostringstream trace_content;
  trace_content << trace_file.rdbuf();

  EXPECT_NE(trace_content.str().find("Word: ab"), std::string::npos);
  // The trace goes to the file, not to out.
  EXPECT_EQ(out.str().find("Word:"), std::string::npos);

  std::remove(automaton_path.c_str());
  std::remove(words_path.c_str());
  std::remove(trace_path.c_str());
}

TEST(ApplicationTest, RunWithUnwritableOutputFileThrowsFileNotFoundException) {
  std::string automaton_path = WriteTempFile("automaton_bad_out", kApvContent);

  Options options;
  options.config_file = automaton_path;
  options.type = AutomatonType::kEmptyStack;
  options.trace = true;
  options.output_file = "/";  // a directory can never be opened for writing

  std::istringstream in("3\n");
  std::ostringstream out;
  Application application(options, in, out);

  EXPECT_THROW(application.Run(), FileNotFoundException);

  std::remove(automaton_path.c_str());
}