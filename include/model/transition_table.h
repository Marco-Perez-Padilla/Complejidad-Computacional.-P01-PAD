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

** File transition_table.h: storage for a pushdown automaton's transitions, with lookup by (state, input symbol, stack top).
**/

#ifndef TRANSITION_TABLE_H_
#define TRANSITION_TABLE_H_

#include <string>
#include <vector>

#include "transition.h"

/**
 * @brief Stores a pushdown automaton's transition function and answers
 * lookups by (state, input symbol, stack top).
 *
 * Because the automaton can be non-deterministic, more than one
 * transition may match the same (state, input, top) combination; Find
 * returns every match.
 */
class TransitionTable {
 public:
  TransitionTable() = default;

  void Add(const Transition& transition);
  std::vector<Transition> All() const { return transitions_; }
  std::vector<Transition> Find(const std::string& state, char input, char top) const;
  size_t Size() const;

 private:
  std::vector<Transition> transitions_;
};

#endif 