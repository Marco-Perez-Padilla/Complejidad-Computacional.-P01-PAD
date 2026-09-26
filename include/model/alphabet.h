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

** File alphabet.h: set of single-character symbols, used for both the
** input alphabet (Sigma) and the stack alphabet (Gamma) of a pushdown
** automaton.
**/

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include <set>

/**
 * @brief Represents a finite set of single-character symbols.
 *
 * Used for both Sigma and Gamma. Centralizes membership checks and
 * rejects the characters reserved by the definition file format ('.'
 * for epsilon and '#' for comments), which can never be symbols of any
 * alphabet.
 */
class Alphabet {
 public:
  Alphabet() = default;

  static bool IsReserved(char symbol);
  bool AddSymbol(char symbol);
  bool Contains(char symbol) const;
  size_t Size() const;
  bool Empty() const;

 private:
  std::set<char> symbols_;
};

#endif 