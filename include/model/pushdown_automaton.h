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

** File pushdown_automaton.h: the formal definition of a pushdown automaton.
**/

#ifndef PUSHDOWN_AUTOMATON_H_
#define PUSHDOWN_AUTOMATON_H_

#include <memory>
#include <set>
#include <string>

#include "include/acceptance/acceptance_criterion.h"
#include "include/model/alphabet.h"
#include "include/model/automaton_type.h"
#include "include/model/transition_table.h"

/**
 * @brief The formal definition of a pushdown automaton: its states,
 * alphabets, initial configuration, transition function and acceptance
 * mode.
 *
 * The constructor validates the definition against the formal
 * requirements of a pushdown automaton (initial state in Q, initial
 * stack symbol in Gamma, every transition referring only to declared
 * states and symbols, final states subset of Q) and throws
 * InvalidDefinitionException if any of them is violated.
 */
class PushdownAutomaton {
 public:
  PushdownAutomaton(std::set<std::string> states, Alphabet input_alphabet,
                     Alphabet stack_alphabet, std::string initial_state,
                     char initial_stack_symbol, TransitionTable transitions,
                     AutomatonType type, std::set<std::string> final_states);

  const std::set<std::string>& States() const;
  const Alphabet& InputAlphabet() const;
  const Alphabet& StackAlphabet() const;
  const std::string& InitialState() const;
  char InitialStackSymbol() const;
  const TransitionTable& Transitions() const;
  AutomatonType Type() const;

  const AcceptanceCriterion& Acceptance() const;

 private:
  void Validate() const;

  std::set<std::string> states_;
  Alphabet input_alphabet_;
  Alphabet stack_alphabet_;
  std::string initial_state_;
  char initial_stack_symbol_;
  TransitionTable transitions_;
  AutomatonType type_;
  std::set<std::string> final_states_;
  std::unique_ptr<AcceptanceCriterion> acceptance_criterion_;
};

#endif 