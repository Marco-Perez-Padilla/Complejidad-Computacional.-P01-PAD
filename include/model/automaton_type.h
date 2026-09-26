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

** File automaton_type.h: the two acceptance modes a pushdown automaton can use.
**/

#ifndef AUTOMATON_TYPE_H_
#define AUTOMATON_TYPE_H_

/**
 * @brief Acceptance mode of a pushdown automaton: by empty stack (APv) or by final state (APf).
 */
enum class AutomatonType {
  kEmptyStack,
  kFinalState,
};

#endif 