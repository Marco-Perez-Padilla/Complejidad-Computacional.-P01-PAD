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

** File configuration.cc: an instantaneous description of a pushdown automaton during simulation.
**/

#include "include/model/configuration.h"

/**
  * @brief A string that uniquely identifies this configuration, used
  * to detect configurations already explored by the search.
*/
std::string Configuration::Key() const {
  return state + '\x1f' + remaining_input + '\x1f' + stack;
}