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

** File simulator.cc: implementation of Simulator.
**/

#include "include/simulation/simulator.h"

#include "include/exceptions/exceptions.h"

/**
  * @brief Builds a simulator for the given automaton.
  * @param automaton Automaton to simulate; must outlive the Simulator.
  * @param max_depth Maximum number of configurations explored along a
  *        single search branch.
  * @param trace_writer If not null, every explored configuration is
  *        reported to it as the search progresses.
*/
Simulator::Simulator(const PushdownAutomaton& automaton, int max_depth, TraceWriter* trace_writer)
    : automaton_(automaton),
      max_depth_(max_depth),
      trace_writer_(trace_writer) {}

/**
  * @brief Checks whether word is recognized by the automaton.
  * @param word Input word over the automaton's input alphabet.
  * @return true if some computation path accepts word.
  * @throws InvalidWordException if word contains a symbol that is not
  *         in the automaton's input alphabet.
*/
bool Simulator::Accepts(const std::string& word) const {
  for (char symbol : word) {
    if (!automaton_.InputAlphabet().Contains(symbol)) {
      throw InvalidWordException(word, symbol);
    }
  }

  if (trace_writer_ != nullptr) {
    trace_writer_->PrintHeader(word, automaton_.Acceptance().Name());
  }

  Configuration initial{automaton_.InitialState(), word,
                         std::string(1, automaton_.InitialStackSymbol())};
  std::set<std::string> visited;
  bool accepted = Explore(initial, &visited, 0);

  if (trace_writer_ != nullptr) {
    trace_writer_->PrintResult(accepted);
  }
  return accepted;
}

/**
 * @brief Explores the search tree rooted at configuration, looking for
 * an accepting configuration.
 * @param configuration Configuration to explore.
 * @param visited Set of configurations already explored along the
 *        current search branch, to avoid infinite epsilon loops.
 * @param depth Current depth in the search tree.
 * @return true if an accepting configuration was found.
 */
bool Simulator::Explore(const Configuration& configuration,
                         std::set<std::string>* visited, int depth) const {
  if (depth > max_depth_ || !visited->insert(configuration.Key()).second) {
    return false;
  }

  if (automaton_.Acceptance().IsAccepting(configuration)) {
    if (trace_writer_ != nullptr) {
      trace_writer_->PrintConfiguration(depth, configuration, {});
      trace_writer_->PrintAccepted(depth);
    }
    return true;
  }

  char top = configuration.stack.empty() ? '\0' : configuration.stack.front();
  std::vector<Transition> applicable;
  if (!configuration.remaining_input.empty()) {
    std::vector<Transition> by_symbol = automaton_.Transitions().Find(
        configuration.state, configuration.remaining_input.front(), top);
    applicable.insert(applicable.end(), by_symbol.begin(), by_symbol.end());
  }
  std::vector<Transition> by_epsilon =
      automaton_.Transitions().Find(configuration.state, kEpsilon, top);
  applicable.insert(applicable.end(), by_epsilon.begin(), by_epsilon.end());

  if (trace_writer_ != nullptr) {
    trace_writer_->PrintConfiguration(depth, configuration, applicable);
  }

  for (const Transition& transition : applicable) {
    Configuration next = configuration;
    next.state = transition.to;
    next.stack = transition.push + configuration.stack.substr(1);
    if (transition.input != kEpsilon) {
      next.remaining_input = configuration.remaining_input.substr(1);
    }
    if (trace_writer_ != nullptr) {
      trace_writer_->PrintTransition(depth, transition);
    }
    if (Explore(next, visited, depth + 1)) return true;
  }

  if (applicable.empty() && trace_writer_ != nullptr) {
    trace_writer_->PrintDeadEnd(depth);
  }
  return false;
}