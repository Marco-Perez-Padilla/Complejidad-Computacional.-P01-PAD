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

** File acceptance_factory.cc: creates the AcceptanceCriterion strategy that corresponds to an automaton type.
**/

#include "include/acceptance/acceptance_factory.h"

#include "include/acceptance/empty_stack_acceptance.h"
#include "include/acceptance/final_state_acceptance.h"

/**
 * @brief Creates an acceptance criterion object based on the given automaton type.
 * @param type The type of the automaton (empty stack or final state).
 * @param final_states A set of final states (only used for final state acceptance).
 * @return A unique pointer to the created acceptance criterion object if the type is valid; 
 * otherwise, returns a unique pointer to an empty stack acceptance criterion.
 */
std::unique_ptr<AcceptanceCriterion> MakeAcceptanceCriterion(
    AutomatonType type, const std::set<std::string>& final_states) {
  if (type == AutomatonType::kFinalState) {
    return std::make_unique<FinalStateAcceptance>(final_states);
  }
  return std::make_unique<EmptyStackAcceptance>();
}