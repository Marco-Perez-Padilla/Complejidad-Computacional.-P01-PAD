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

** File configuration.h: an instantaneous description of a pushdown automaton during simulation.
**/

#ifndef _CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

/**
 * @brief The state of a pushdown automaton at one point of the
 * simulation: its current state, the input still to be consumed, and
 * the stack content with the top symbol first.
 */
struct Configuration {
  std::string state;
  std::string remaining_input;
  std::string stack;

  std::string Key() const;
};

#endif  