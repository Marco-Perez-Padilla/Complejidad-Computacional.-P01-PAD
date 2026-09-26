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

** File final_state_acceptance.h: acceptance-by-final-state strategy.
**/

#ifndef FINAL_STATE_ACCEPTANCE_H_
#define FINAL_STATE_ACCEPTANCE_H_

#include <set>
#include <string>

#include "acceptance_criterion.h"

/**
 * @brief Acceptance by final state: a word is accepted when the input
 * has been fully consumed and the current state is one of the
 * automaton's final states. The stack content is irrelevant.
 */
class FinalStateAcceptance : public AcceptanceCriterion {
 public:
  explicit FinalStateAcceptance(std::set<std::string> final_states);

  bool IsAccepting(const Configuration& configuration) const override;
  std::string Name() const override;

 private:
  std::set<std::string> final_states_;
};

#endif 