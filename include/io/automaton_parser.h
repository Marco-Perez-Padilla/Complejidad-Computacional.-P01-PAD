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

** File automaton_parser.h: reads a pushdown automaton definition from a text file.
**/

#ifndef AUTOMATON_PARSER_H_
#define AUTOMATON_PARSER_H_

#include <string>

#include "include/model/pushdown_automaton.h"

PushdownAutomaton ParseAutomatonFile(const std::string& filename);

#endif 