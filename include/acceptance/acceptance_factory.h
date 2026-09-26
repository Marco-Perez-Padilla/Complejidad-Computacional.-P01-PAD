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

** File acceptance_factory.h: creates the AcceptanceCriterion strategy that corresponds to an automaton type.
**/

#ifndef ACCEPTANCE_FACTORY_H_
#define ACCEPTANCE_FACTORY_H_

#include <memory>
#include <set>
#include <string>

#include "acceptance_criterion.h"
#include "include/model/automaton_type.h"

/**
 * @brief Creates the AcceptanceCriterion strategy for an automaton type.
 * @param type Acceptance mode of the automaton.
 * @param final_states Set of final states; only used when type is
 *        AutomatonType::kFinalState.
 * @return A new AcceptanceCriterion implementing that mode.
 */
std::unique_ptr<AcceptanceCriterion> MakeAcceptanceCriterion(
    AutomatonType type, const std::set<std::string>& final_states);

#endif 