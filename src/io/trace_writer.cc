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

** File trace_writer.cc: formats the simulator's depth-first search as a readable trace.
**/

#include "include/io/trace_writer.h"

namespace {

/**
  * @brief Formats a symbol for printing, using "eps" for the epsilon symbol.
  * @param symbol The symbol to format.
  * @return A string representation of the symbol.
*/
std::string FormatSymbol(char symbol) {
  return symbol == kEpsilon ? "eps" : std::string(1, symbol);
}

/**
 * @brief Formats a string for printing, using "eps" for an empty string.
 * @param text The string to format.
 * @return A string representation of the text.
 */
std::string FormatOrEpsilon(const std::string& text) {
  return text.empty() ? "eps" : text;
}

}  // namespace

/**
 * @brief Constructs a TraceWriter instance.
 * @param out The output stream to write the trace to.
 */
TraceWriter::TraceWriter(std::ostream& out) : out_(out) {}

/**
 * @brief Returns a string of spaces for indentation based on the depth in the DFS.
 * @param depth The depth in the DFS.
 * @return A string of spaces for indentation.
 */
std::string TraceWriter::Indent(int depth) const {
  return std::string(static_cast<size_t>(depth) * 2, ' ');
}

/**
  * @brief Prints the header shown once per simulated word.
*/
void TraceWriter::PrintHeader(const std::string& word,
                               const std::string& acceptance_name) {
  out_ << "Word: " << FormatOrEpsilon(word) << "  [acceptance by "
       << acceptance_name << "]\n";
}

/**
  * @brief Prints one visited configuration, at the given DFS depth, and
  * how many transitions are applicable from it.
*/
void TraceWriter::PrintConfiguration(
    int depth, const Configuration& configuration,
    const std::vector<Transition>& applicable) {
  out_ << Indent(depth) << "(" << configuration.state << ", "
       << FormatOrEpsilon(configuration.remaining_input) << ", "
       << FormatOrEpsilon(configuration.stack) << ")";
  if (applicable.empty()) {
    out_ << "  no applicable transitions\n";
  } else {
    out_ << "  applicable: " << applicable.size() << "\n";
  }
}

/**
  * @brief Prints that a transition is being applied to move deeper into the search.
*/
void TraceWriter::PrintTransition(int depth, const Transition& transition) {
  out_ << Indent(depth) << "-> (" << transition.from << ", "
       << FormatSymbol(transition.input) << ", " << transition.top
       << ") => (" << transition.to << ", "
       << FormatOrEpsilon(transition.push) << ")\n";
}

/**
  * @brief Prints that the configuration just shown is accepting.
*/
void TraceWriter::PrintAccepted(int depth) {
  out_ << Indent(depth) << "ACCEPTED\n";
}

/**
  * @brief Prints that a branch has no applicable transitions and is abandoned.
*/
void TraceWriter::PrintDeadEnd(int depth) {
  out_ << Indent(depth) << "dead end\n";
}

/**
  * @brief Prints the final verdict for the simulated word.
*/
void TraceWriter::PrintResult(bool accepted) {
  out_ << (accepted ? "Result: ACCEPTED" : "Result: REJECTED") << "\n\n";
}