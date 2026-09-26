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

** File alphabet.cc: set of single-character symbols, used for both the
** input alphabet (Sigma) and the stack alphabet (Gamma) of a pushdown
** automaton.
**/

#include "include/model/alphabet.h"

/**
  * @brief Checks whether a character cannot belong to any alphabet
  * because it is reserved by the file format.
  * @param symbol Character to check.
  * @return true if symbol is '.' (epsilon) or '#' (comment).
*/
bool Alphabet::IsReserved(char symbol) {
  return symbol == '.' || symbol == '#';
}

/**
  * @brief Adds a symbol to the alphabet, unless it is reserved.
  * @param symbol Character to add.
  * @return true if the symbol was added (or already present); false if
  *         symbol is reserved and therefore was not added.
*/
bool Alphabet::AddSymbol(char symbol) {
  if (IsReserved(symbol)) return false;
  symbols_.insert(symbol);
  return true;
}

/**
  * @brief Checks whether a symbol belongs to the alphabet.
  * @param symbol Character to check.
  * @return true if the symbol is in the alphabet; false otherwise.
*/
bool Alphabet::Contains(char symbol) const {
  return symbols_.find(symbol) != symbols_.end();
}

/**
  * @brief Number of distinct symbols in the alphabet.
  * @return Number of symbols.
*/
size_t Alphabet::Size() const { return symbols_.size(); }


/**
 * @brief Checks whether the alphabet has no symbols.
 * @return true if the alphabet is empty; false otherwise.
 */
bool Alphabet::Empty() const { return symbols_.empty(); }