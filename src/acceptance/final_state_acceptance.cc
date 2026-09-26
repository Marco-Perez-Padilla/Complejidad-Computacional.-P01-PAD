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

** File final_state_acceptance.cc: acceptance-by-final-state strategy.
**/

#include "include/acceptance/final_state_acceptance.h"

/**
 * @brief Constructs a FinalStateAcceptance object with the given set of final states.
 * @param final_states A set of strings representing the final states of the automaton.
 */
FinalStateAcceptance::FinalStateAcceptance(std::set<std::string> final_states)
    : final_states_(std::move(final_states)) {}

/**
 * @brief Checks whether the given configuration is an accepting configuration based on the final state acceptance criterion.
 * @param configuration The configuration to check.
 * @return true if the configuration is accepting (input fully consumed and current state is a final state), false otherwise.
 */
bool FinalStateAcceptance::IsAccepting(
    const Configuration& configuration) const {
  return configuration.remaining_input.empty() &&
         final_states_.find(configuration.state) != final_states_.end();
}

/**
 * @brief Returns the name of the acceptance criterion.
 * @return The name of the acceptance criterion.
 */
std::string FinalStateAcceptance::Name() const { return "final state"; }