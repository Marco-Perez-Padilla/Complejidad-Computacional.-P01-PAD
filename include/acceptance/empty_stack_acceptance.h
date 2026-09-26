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

** File empty_stack_acceptance.h: acceptance-by-empty-stack strategy.
**/

#ifndef EMPTY_STACK_ACCEPTANCE_H_
#define EMPTY_STACK_ACCEPTANCE_H_

#include "acceptance_criterion.h"

/**
 * @brief Acceptance by empty stack: a word is accepted when the input
 * has been fully consumed and the stack is empty. The current state is
 * irrelevant.
 */
class EmptyStackAcceptance : public AcceptanceCriterion {
 public:
  bool IsAccepting(const Configuration& configuration) const override;
  std::string Name() const override;
};

#endif