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

** File simulator.h: depth-first search simulation of a pushdown automaton.
**/

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <set>
#include <string>

#include "include/io/trace_writer.h"
#include "include/model/pushdown_automaton.h"

/**
 * @brief Simulates a PushdownAutomaton on an input word with a
 * depth-first search over its configurations.
 *
 * Transitions that consume an input symbol are tried before epsilon
 * transitions. A visited-configuration set prevents infinite epsilon
 * loops, and a maximum search depth guards against unbounded stack
 * growth; reaching it makes the current branch be treated as rejected.
 */
class Simulator {
 public:  
  explicit Simulator(const PushdownAutomaton& automaton, int max_depth = 500, TraceWriter* trace_writer = nullptr);

  bool Accepts(const std::string& word) const;

 private:
  bool Explore(const Configuration& configuration, std::set<std::string>* visited, int depth) const;

  const PushdownAutomaton& automaton_;
  int max_depth_;
  TraceWriter* trace_writer_;
};

#endif