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

** File menu.h: interactive loop that lets the user check words against a  WordChecker, from the keyboard or from a file.
**/

#ifndef MENU_H_
#define MENU_H_

#include <istream>
#include <ostream>
#include <string>

#include "word_checker.h"

/**
 * @brief Interactive loop that lets the user check whether words belong to
 * the language of an automaton, entering them either from the keyboard or
 * from a file, until they choose to exit.
 */
class Menu {
 public:
  Menu(const WordChecker& checker, std::istream& in, std::ostream& out);
  void Run();

 private:
  void RunKeyboardMode();
  void RunFileMode();

  const WordChecker& checker_;
  std::istream& in_;
  std::ostream& out_;
};

#endif 