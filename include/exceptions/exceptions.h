/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Subject: 'Complejidad Computacional'
** Course: 4º
** Practice 1: Pushdown automaton
** Author: Marco Pérez Padilla
** Email: alu0101469348@ull.edu.es
** Date: 25/09/2026

** File exceptions.h: exception hierarchy for the pushdown automaton simulator.
**/

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>

/**
 * @brief Base class for every exception thrown by the simulator.
 */
class PdaException : public std::exception {
 public:
  explicit PdaException(const std::string& message) : message_(message) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

/**
 * @brief Thrown when a file requested by the user cannot be opened
 * (automaton definition file or input strings file).
 */
class FileNotFoundException : public PdaException {
 public:
  explicit FileNotFoundException(const std::string& filename)
      : PdaException("cannot open file '" + filename + "'") {}
};

/**
 * @brief Thrown when a file exists but is empty or has no valid content.
 */
class EmptyFileException : public PdaException {
 public:
  explicit EmptyFileException(const std::string& filename)
      : PdaException("file '" + filename + "' is empty or has no valid content") {}
};

/**
 * @brief Thrown when the command-line arguments are incorrect or incomplete.
 */
class InvalidArgumentsException : public PdaException {
 public:
  explicit InvalidArgumentsException(const std::string& reason)
      : PdaException("invalid arguments: " + reason) {}
};

/**
 * @brief Thrown when the automaton definition violates the formal
 * definition of a pushdown automaton (e.g. the initial state is not in Q).
 */
class InvalidDefinitionException : public PdaException {
 public:
  InvalidDefinitionException(int line, const std::string& reason)
      : PdaException("line " + std::to_string(line) + ": invalid definition: " + reason) {}
};

/**
 * @brief Thrown when an input word contains a symbol that does not
 * belong to the automaton's input alphabet (Sigma).
 */
class InvalidWordException : public PdaException {
 public:
  InvalidWordException(const std::string& word, char symbol)
      : PdaException("word '" + word + "' contains symbol '" + std::string(1, symbol) + "' which is not in Sigma") {}
};

#endif  // EXCEPTIONS_H_