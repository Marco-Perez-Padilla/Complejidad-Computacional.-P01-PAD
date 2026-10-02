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

** File application.cc: top-level orchestration invoked by main after the command-line arguments are validated.
**/

#include "io/application.h"

#include <fstream>
#include <utility>

#include "include/exceptions/exceptions.h"
#include "include/io/automaton_parser.h"
#include "include/io/menu.h"
#include "include/io/word_checker.h"

/**
  * @brief Builds an application bound to already-validated options and to
  * the streams used for interaction. in/out default to std::cin/std::cout
  * so production code doesn't have to pass them explicitly.
*/
Application::Application(Options options, std::istream& in, std::ostream& out)
    : options_(std::move(options)), in_(in), out_(out) {}

PushdownAutomaton Application::LoadAutomaton() const {
  PushdownAutomaton automaton =
      ParseAutomatonFile(options_.config_file, options_.type);
  out_ << "Automaton loaded: " << automaton.States().size()
       << " states, acceptance by " << automaton.Acceptance().Name() << "\n";
  return automaton;
}

/**
 * @brief Opens trace_file on options_.output_file when tracing to a file was
 * requested and returns a reference to whichever stream the trace should
 * go to. trace_file must outlive the returned reference, so it is owned
 * by the caller (Run), not by this method.
 * @param trace_file An ofstream that will be opened on options_.output_file if
 *        tracing to a file was requested. Must outlive the returned reference.
 * @return A reference to the stream to which the trace should be written.
 * @throws FileNotFoundException if the output file cannot be opened.
 */
std::ostream& Application::SelectTraceStream(std::ofstream& trace_file) const {
  if (options_.trace && options_.output_file.has_value()) {
    trace_file.open(*options_.output_file);
    if (!trace_file.is_open()) {
      throw FileNotFoundException(*options_.output_file);
    }
    return trace_file;
  }
  return out_;
}

/**
  * @brief Loads the automaton, opens the trace output if requested, and
  * checks words against it: from options.input_file if given, otherwise
  * interactively through a Menu.
  * @throws PdaException (or a subclass) if the automaton definition, the  input file or the output file cannot be processed.
*/
void Application::Run() {
  PushdownAutomaton automaton = LoadAutomaton();

  std::ofstream trace_file;
  std::ostream& trace_out = SelectTraceStream(trace_file);

  WordChecker checker(automaton, options_.trace, out_, trace_out);

  if (options_.input_file.has_value()) {
    checker.CheckAllFromFile(*options_.input_file);
  } else {
    Menu menu(checker, in_, out_);
    menu.Run();
  }
}