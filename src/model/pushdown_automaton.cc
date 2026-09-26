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

** File pushdown_automaton.cc: the formal definition of a pushdown automaton.
**/

#include "include/model/pushdown_automaton.h"

#include "include/acceptance/acceptance_factory.h"
#include "include/exceptions/exceptions.h"

/**
 * @brief Constructs a pushdown automaton from its formal definition.
 * @param states The set of states Q.
 * @param input_alphabet The input alphabet Sigma.
 * @param stack_alphabet The stack alphabet Gamma.
 * @param initial_state The initial state q0.
 * @param initial_stack_symbol The initial stack symbol Z0.
 * @param transitions The transition function delta.
 * @param type The acceptance mode (final state or empty stack).
 * @param final_states The set of final states F (ignored if type is empty stack).
 * @throws InvalidDefinitionException if the definition is not well-formed.
 */
PushdownAutomaton::PushdownAutomaton(std::set<std::string> states,
                                      Alphabet input_alphabet,
                                      Alphabet stack_alphabet,
                                      std::string initial_state,
                                      char initial_stack_symbol,
                                      TransitionTable transitions,
                                      AutomatonType type,
                                      std::set<std::string> final_states)
    : states_(std::move(states)),
      input_alphabet_(std::move(input_alphabet)),
      stack_alphabet_(std::move(stack_alphabet)),
      initial_state_(std::move(initial_state)),
      initial_stack_symbol_(initial_stack_symbol),
      transitions_(std::move(transitions)),
      type_(type),
      final_states_(std::move(final_states)) {
  Validate();
  acceptance_criterion_ = MakeAcceptanceCriterion(type_, final_states_);
}

/**
 * @brief Checks the formal well-formedness of the automaton definition.
 */
void PushdownAutomaton::Validate() const {
  if (states_.empty()) {
    throw InvalidDefinitionException(0, "Q must not be empty");
  }
  if (states_.find(initial_state_) == states_.end()) {
    throw InvalidDefinitionException(
        0, "initial state '" + initial_state_ + "' is not in Q");
  }
  if (!stack_alphabet_.Contains(initial_stack_symbol_)) {
    throw InvalidDefinitionException(
        0, "initial stack symbol '" + std::string(1, initial_stack_symbol_) +
               "' is not in Gamma");
  }
  for (const std::string& final_state : final_states_) {
    if (states_.find(final_state) == states_.end()) {
      throw InvalidDefinitionException(
          0, "final state '" + final_state + "' is not in Q");
    }
  }
  for (const Transition& transition : transitions_.All()) {
    if (states_.find(transition.from) == states_.end()) {
      throw InvalidDefinitionException(
          0, "transition origin state '" + transition.from + "' is not in Q");
    }
    if (transition.input != kEpsilon &&
        !input_alphabet_.Contains(transition.input)) {
      throw InvalidDefinitionException(
          0, "transition input symbol '" + std::string(1, transition.input) +
                 "' is not in Sigma");
    }
    if (!stack_alphabet_.Contains(transition.top)) {
      throw InvalidDefinitionException(
          0, "transition top-of-stack symbol '" +
                 std::string(1, transition.top) + "' is not in Gamma");
    }
    if (states_.find(transition.to) == states_.end()) {
      throw InvalidDefinitionException(
          0, "transition destination state '" + transition.to +
                 "' is not in Q");
    }
    for (char symbol : transition.push) {
      if (!stack_alphabet_.Contains(symbol)) {
        throw InvalidDefinitionException(
            0, "pushed symbol '" + std::string(1, symbol) +
                   "' is not in Gamma");
      }
    }
  }
}

/**
 * @brief The set of states Q.
 * @return The set of states Q.
 */
const std::set<std::string>& PushdownAutomaton::States() const {
  return states_;
}

/**
 * @brief The input alphabet Sigma.
 * @return The input alphabet Sigma.
 */
const Alphabet& PushdownAutomaton::InputAlphabet() const {
  return input_alphabet_;
}

/**
 * @brief The stack alphabet Gamma.
 * @return The stack alphabet Gamma.
 */
const Alphabet& PushdownAutomaton::StackAlphabet() const {
  return stack_alphabet_;
}

/**
 * @brief The initial state q0.
 * @return The initial state q0.
 */
const std::string& PushdownAutomaton::InitialState() const {
  return initial_state_;
}

/**
 * @brief The initial stack symbol Z0.
 * @return The initial stack symbol Z0.
 */
char PushdownAutomaton::InitialStackSymbol() const {
  return initial_stack_symbol_;
}

/**
 * @brief The transition function delta.
 * @return The transition function delta.
 */
const TransitionTable& PushdownAutomaton::Transitions() const {
  return transitions_;
}

/**
 * @brief The acceptance mode (final state or empty stack).
 * @return The acceptance mode (final state or empty stack).
 */
AutomatonType PushdownAutomaton::Type() const { return type_; }

/**
  * @brief The acceptance strategy of this automaton (final state or
  * empty stack, chosen by MakeAcceptanceCriterion when this automaton
  * was constructed).
  * @return The acceptance criterion of this automaton.
*/
const AcceptanceCriterion& PushdownAutomaton::Acceptance() const {
  return *acceptance_criterion_;
}