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

** File word_checker.h: checks whether words belong to the language of a PushdownAutomaton, optionally tracing the simulation.
**/

#ifndef WORD_CHECKER_H_
#define WORD_CHECKER_H_

#include <ostream>
#include <string>

#include "include/model/pushdown_automaton.h"

class WordChecker {
 public:
  WordChecker(const PushdownAutomaton& automaton, bool trace, std::ostream& out, std::ostream& trace_out);

  
  void Check(const std::string& word, int line_number = -1) const;

  void CheckAllFromFile(const std::string& filename) const;

 private:
  const PushdownAutomaton& automaton_;
  bool trace_;
  std::ostream& out_;
  std::ostream& trace_out_;
};

#endif  