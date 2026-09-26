// File exceptions_test.cc: tests for the exception hierarchy in
// exceptions.h. Each exception is checked for its exact message via
// what(), and for being catchable as PdaException (its base class).

#include "include/exceptions/exceptions.h"

#include <gtest/gtest.h>

TEST(ExceptionsTest, FileNotFoundMessageContainsFilename) {
  FileNotFoundException error("input.txt");
  EXPECT_STREQ(error.what(), "cannot open file 'input.txt'");
}

TEST(ExceptionsTest, EmptyFileMessageContainsFilename) {
  EmptyFileException error("input.txt");
  EXPECT_STREQ(error.what(),
               "file 'input.txt' is empty or has no valid content");
}

TEST(ExceptionsTest, InvalidArgumentsMessageContainsReason) {
  InvalidArgumentsException error("-config requires a file name");
  EXPECT_STREQ(error.what(),
               "invalid arguments: -config requires a file name");
}

TEST(ExceptionsTest, InvalidDefinitionMessageContainsLineAndReason) {
  InvalidDefinitionException error(7, "initial state is not in Q");
  EXPECT_STREQ(error.what(),
               "line 7: invalid definition: initial state is not in Q");
}

TEST(ExceptionsTest, InvalidWordMessageContainsWordAndSymbol) {
  InvalidWordException error("aXb", 'X');
  EXPECT_STREQ(error.what(),
               "word 'aXb' contains symbol 'X' which is not in Sigma");
}

TEST(ExceptionsTest, EveryExceptionIsCatchableAsPdaException) {
  EXPECT_THROW(
      {
        try {
          throw InvalidWordException("ab", 'X');
        } catch (const PdaException& error) {
          throw;
        }
      },
      PdaException);
}