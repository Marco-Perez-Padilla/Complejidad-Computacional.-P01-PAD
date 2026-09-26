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

** File word_checker.cc: checks whether words belong to the language of a PushdownAutomaton, optionally tracing the simulation.
**/

#include "io/word_checker.h"

#include <fstream>

#include "include/exceptions/exceptions.h"
#include "include/help/help_functions.h"
#include "include/io/trace_writer.h"
#include "include/simulation/simulator.h"

namespace {
constexpr const char kEmptyWordToken[] = ".";
}  // namespace

WordChecker::WordChecker(const PushdownAutomaton& automaton, bool trace,
                          std::ostream& out, std::ostream& trace_out)
    : automaton_(automaton), trace_(trace), out_(out), trace_out_(trace_out) {}

/**
  * @brief Checks a single word and prints the result to out. '.' is
  * treated as the empty word, matching the file/keyboard input format.
  * @param word Word to check.
  * @param line_number If non-negative, the line this word came from in
  *        an input file, reported alongside a warning for a word with
  *        a symbol outside Sigma. Left at -1 for words typed directly
  *        at the keyboard, which have no line to report.
*/
void WordChecker::Check(const std::string& word, int line_number) const {
  const std::string actual_word = word == kEmptyWordToken ? "" : word;

  TraceWriter writer(trace_out_);
  Simulator simulator(automaton_, 500, trace_ ? &writer : nullptr);
  try {
    bool accepted = simulator.Accepts(actual_word);
    out_ << "'" << (actual_word.empty() ? "eps" : actual_word) << "': "
         << (accepted ? "ACCEPTED" : "REJECTED") << "\n";
  } catch (const InvalidWordException& e) {
    if (line_number >= 0) {
      PrintWarning("line " + std::to_string(line_number) + ": " + e.what());
    } else {
      PrintWarning(e.what());
    }
  }
}

/**
 * @brief Checks all words in a file, printing the result for each to out.
 * @param filename Name of the file containing one word per line.
 * @throws FileNotFoundException if the file cannot be opened.
 */
void WordChecker::CheckAllFromFile(const std::string& filename) const {
  std::ifstream file(filename);
  if (!file.is_open()) throw FileNotFoundException(filename);

  std::string word;
  int line_number = 0;
  while (std::getline(file, word)) {
    ++line_number;
    if (word.empty()) continue;
    Check(word, line_number);
  }
}