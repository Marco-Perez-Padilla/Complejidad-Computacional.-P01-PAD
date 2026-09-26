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

** File empty_stack_acceptance.cc: acceptance-by-empty-stack strategy.
**/

#include "include/acceptance/empty_stack_acceptance.h"

/**
 * @brief Returns true if the configuration is accepting by empty stack.
 * @param configuration The configuration to check.
 * @return true if the configuration is accepting by empty stack, false otherwise.
 */
bool EmptyStackAcceptance::IsAccepting(
    const Configuration& configuration) const {
  return configuration.remaining_input.empty() && configuration.stack.empty();
}

/**
 * @brief Returns the name of the acceptance criterion.
 * @return The name of the acceptance criterion.
 */
std::string EmptyStackAcceptance::Name() const { return "empty stack"; }