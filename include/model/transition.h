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

** File transition.h: representation of a pushdown automaton transition.
**/

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <string>

/**
 * @brief Internal marker used to represent epsilon as the input symbol
 * of a transition. It reuses the definition file's own epsilon
 * character ('.'), which is guaranteed to never collide with a real
 * Sigma symbol because Alphabet::IsReserved rejects it.
 */
inline constexpr char kEpsilon = '.';

/**
 * @brief A pushdown automaton transition: delta(from, input, top)
 * contains (to, push).
 *
 * input is a symbol of Sigma, or kEpsilon for an epsilon transition on
 * the input tape. push is the string of Gamma symbols that replaces top
 * on the stack, written left to right with the first symbol ending up
 * on top; an empty string means popping top without pushing anything
 * back.
 */
struct Transition {
  std::string from;
  char input = kEpsilon;
  char top = '\0';
  std::string to;
  std::string push;
};

#endif 