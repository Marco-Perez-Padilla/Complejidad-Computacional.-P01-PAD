// File help_functions_test.cc: tests for Help, Usage, PrintWarning,
// PrintError and ValidateArguments.
//
// Help/Usage/PrintWarning/PrintError only print to stdout/stderr, so
// they are checked with GoogleTest's stdout/stderr capture instead of
// asserting on a return value.

#include "include/help/help_functions.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace {

// Builds a char* argv[] (and its matching argc) from a list of strings,
// the shape ValidateArguments expects. argument_storage must outlive
// the returned pointers, so the caller keeps it alive on the stack.
std::vector<char*> MakeArgv(std::vector<std::string>& argument_storage) {
  std::vector<char*> argv;
  for (std::string& argument : argument_storage) {
    argv.push_back(argument.data());
  }
  return argv;
}

}  // namespace

TEST(HelpFunctionsTest, HelpPrintsUsageLineToStdout) {
  testing::internal::CaptureStdout();
  Help();
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_NE(output.find("-config"), std::string::npos);
}

TEST(HelpFunctionsTest, UsagePrintsDefinitionFileFormatToStdout) {
  testing::internal::CaptureStdout();
  Usage();
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_NE(output.find("Sigma"), std::string::npos);
  EXPECT_NE(output.find("Gamma"), std::string::npos);
}

TEST(HelpFunctionsTest, PrintWarningGoesToStderrWithPrefix) {
  testing::internal::CaptureStderr();
  PrintWarning("something odd");
  std::string output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(output, "Warning: something odd\n");
}

TEST(HelpFunctionsTest, PrintErrorGoesToStderrWithPrefix) {
  testing::internal::CaptureStderr();
  PrintError("something wrong");
  std::string output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(output, "Error: something wrong\n");
}

TEST(ValidateArgumentsTest, HelpFlagPrintsHelpAndReturnsZero) {
  std::vector<std::string> argument_storage{"pda_simulator", "--help"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(status, 0);
  EXPECT_NE(output.find("Usage:"), std::string::npos);
}

TEST(ValidateArgumentsTest, MissingConfigReturnsOne) {
  std::vector<std::string> argument_storage{"pda_simulator", "-trace"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  testing::internal::GetCapturedStdout();
  std::string error_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, 1);
  EXPECT_NE(error_output.find("-config"), std::string::npos);
}

TEST(ValidateArgumentsTest, ConfigWithoutFileNameReturnsOne) {
  std::vector<std::string> argument_storage{"pda_simulator", "-config"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  testing::internal::GetCapturedStdout();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, 1);
}

TEST(ValidateArgumentsTest, UnknownOptionReturnsOne) {
  std::vector<std::string> argument_storage{"pda_simulator", "-config",
                                             "data.txt", "-bogus"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  testing::internal::GetCapturedStdout();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, 1);
}

TEST(ValidateArgumentsTest, ValidArgumentsFillOptionsAndReturnMinusOne) {
  std::vector<std::string> argument_storage{
      "pda_simulator", "-config", "data.txt", "-type",
      "apv",           "-trace",  "-in",      "words.txt"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  int status =
      ValidateArguments(static_cast<int>(argv.size()), argv.data(), &options);

  EXPECT_EQ(status, -1);
  EXPECT_EQ(options.config_file, "data.txt");
  EXPECT_EQ(options.type, AutomatonType::kEmptyStack);
  EXPECT_TRUE(options.trace);
  ASSERT_TRUE(options.input_file.has_value());
  EXPECT_EQ(*options.input_file, "words.txt");
  EXPECT_FALSE(options.output_file.has_value());
}

TEST(ValidateArgumentsTest, TypeApfIsParsedAsFinalState) {
  std::vector<std::string> argument_storage{"pda_simulator", "-config",
                                             "data.txt", "-type", "apf"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  int status =
      ValidateArguments(static_cast<int>(argv.size()), argv.data(), &options);

  EXPECT_EQ(status, -1);
  EXPECT_EQ(options.type, AutomatonType::kFinalState);
}

TEST(ValidateArgumentsTest, MissingTypeReturnsOne) {
  std::vector<std::string> argument_storage{"pda_simulator", "-config",
                                             "data.txt"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  testing::internal::GetCapturedStdout();
  std::string error_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, 1);
  EXPECT_NE(error_output.find("-type"), std::string::npos);
}

TEST(ValidateArgumentsTest, TypeWithoutValueReturnsOne) {
  std::vector<std::string> argument_storage{"pda_simulator", "-config",
                                             "data.txt", "-type"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  testing::internal::GetCapturedStdout();
  testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, 1);
}

TEST(ValidateArgumentsTest, InvalidTypeValueReturnsOne) {
  std::vector<std::string> argument_storage{"pda_simulator", "-config",
                                             "data.txt", "-type", "bogus"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  testing::internal::CaptureStdout();
  int status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                  &options);
  testing::internal::GetCapturedStdout();
  std::string error_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, 1);
  EXPECT_NE(error_output.find("bogus"), std::string::npos);
}

// -out only makes sense together with -trace; without it, it should be
// dropped with a warning rather than silently kept.
TEST(ValidateArgumentsTest, OutputFileWithoutTraceIsDroppedWithWarning) {
  std::vector<std::string> argument_storage{
      "pda_simulator", "-config", "data.txt",
      "-type",         "apv",     "-out", "trace.txt"};
  std::vector<char*> argv = MakeArgv(argument_storage);
  Options options;

  testing::internal::CaptureStderr();
  int status =
      ValidateArguments(static_cast<int>(argv.size()), argv.data(), &options);
  std::string warning_output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(status, -1);
  EXPECT_FALSE(options.output_file.has_value());
  EXPECT_NE(warning_output.find("-out"), std::string::npos);
}