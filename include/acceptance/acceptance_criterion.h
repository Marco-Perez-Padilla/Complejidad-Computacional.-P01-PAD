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

** File acceptance_criterion.h: strategy interface for deciding whether a configuration is accepting.
**/

#ifndef ACCEPTANCE_CRITERION_H_
#define ACCEPTANCE_CRITERION_H_

#include <string>

#include "include/model/configuration.h"

/**
 * @brief Strategy interface for the two standard pushdown automaton
 * acceptance modes: by empty stack and by final state.
 */
class AcceptanceCriterion {
 public:
  virtual ~AcceptanceCriterion() = default;

  /**
   * @brief Checks whether configuration is an accepting configuration.
   */
  virtual bool IsAccepting(const Configuration& configuration) const = 0;

  /**
   * @brief A short human-readable name of the acceptance mode, used in
   * the trace and result output.
   */
  virtual std::string Name() const = 0;
};

#endif 