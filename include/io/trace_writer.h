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

** File trace_writer.h: formats the simulator's depth-first search as a readable trace.
**/

#ifndef TRACE_WRITER_H_
#define TRACE_WRITER_H_

#include <ostream>
#include <string>
#include <vector>

#include "include/model/configuration.h"
#include "include/model/transition.h"

/**
 * @brief Formats the depth-first search performed by Simulator as a
 * readable, indented trace written to an output stream.
 *
 * Kept separate from Simulator so that changing how the trace looks
 * never requires touching the search logic itself.
 */
class TraceWriter {
 public:
  explicit TraceWriter(std::ostream& out);

  void PrintHeader(const std::string& word, const std::string& acceptance_name);
  void PrintConfiguration(int depth, const Configuration& configuration, const std::vector<Transition>& applicable);
  void PrintTransition(int depth, const Transition& transition);
  void PrintAccepted(int depth);
  void PrintDeadEnd(int depth);
  void PrintResult(bool accepted);

 private:
  std::string Indent(int depth) const;

  std::ostream& out_;
};

#endif