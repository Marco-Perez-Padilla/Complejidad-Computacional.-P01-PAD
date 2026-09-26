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

** File transition_table.cc: storage for a pushdown automaton's transitions, with lookup by (state, input symbol, stack top).
**/

#include "include/model/transition_table.h"

/**
  * @brief Adds a transition to the table.
  * @param transition Transition to add.
*/
void TransitionTable::Add(const Transition& transition) {transitions_.push_back(transition);}

/**
  * @brief Finds every transition applicable from a state, with a given
  * input symbol (kEpsilon included) and a given stack top.
  * @param state Source state.
  * @param input Input symbol consumed, or kEpsilon.
  * @param top Symbol expected at the top of the stack.
  * @return The matching transitions, in insertion order; empty if none match.
*/
std::vector<Transition> TransitionTable::Find(const std::string& state, char input, char top) const {
  std::vector<Transition> matches;
  for (const Transition& transition : transitions_) {
    if (transition.from == state && transition.input == input &&
        transition.top == top) {
      matches.push_back(transition);
    }
  }
  return matches;
}

/**
  * @brief Number of transitions stored.
  * @return Number of transitions stored.
*/
size_t TransitionTable::Size() const { return transitions_.size(); }