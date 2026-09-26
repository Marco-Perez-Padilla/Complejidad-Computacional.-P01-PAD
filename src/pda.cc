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

** File pda.cc: Client program for simulating a pushdown automaton. It validates the command-line arguments, 
** reads the automaton definition and input words, and reports whether each word is accepted.
**/

#include "include/exceptions/exceptions.h"
#include "include/help/help_functions.h"
#include "include/io/application.h"
#include "include/io/options.h"

int main(int argc, char* argv[]) {
  Options options;
  int status = ValidateArguments(argc, argv, &options);
  if (status != -1) return status;

  try {
    Application(options).Run();
  } catch (const PdaException& error) {
    PrintError(error.what());
    return 1;
  }
  return 0;
}