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

** File application.h: top-level orchestration invoked by main after the command-line arguments are validated.
**/

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <fstream>
#include <iostream>

#include "options.h"
#include "include/model/pushdown_automaton.h"

/**
 * @brief Loads the automaton described by an Options, then checks words
 * against it: from options.input_file if given, otherwise interactively
 * through a Menu. Writes the trace to options.output_file when requested,
 * or to standard output otherwise.
 */
class Application {
 public:
  explicit Application(Options options, std::istream& in = std::cin, std::ostream& out = std::cout);
  
  void Run();

 private:
  // Parses options_.config_file and reports a one-line summary to out_.
  PushdownAutomaton LoadAutomaton() const;
  std::ostream& SelectTraceStream(std::ofstream& trace_file) const;
  
  Options options_;
  std::istream& in_;
  std::ostream& out_;
};

#endif 