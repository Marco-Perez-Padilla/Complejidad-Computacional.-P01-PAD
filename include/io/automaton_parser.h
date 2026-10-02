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

/**
 * @brief Reads a pushdown automaton definition from filename and builds
 * the corresponding PushdownAutomaton, interpreted as the given type.
 *
 * The type is not guessed from the file: the definition format is
 * ambiguous when the set of final states is empty or missing (it then
 * looks exactly like an empty-stack automaton with no set of final
 * states at all), so the caller must say which one it is. Once known,
 * the line right after the initial stack symbol is read strictly as
 * that type expects: the set of final states for kFinalState, or the
 * first transition for kEmptyStack. A file that does not match the
 * requested type fails with a clear InvalidDefinitionException instead
 * of being silently misread as the other type.
 *
 * @param filename Path to the definition file.
 * @param type Whether filename defines an APv (kEmptyStack) or an APf
 *        (kFinalState) automaton.
 * @return The parsed, validated automaton.
 * @throws FileNotFoundException if filename cannot be opened.
 * @throws EmptyFileException if the file has no meaningful content.
 * @throws InvalidDefinitionException if the definition is malformed,
 *         does not match the requested type, or violates the formal
 *         definition of a pushdown automaton.
 */
PushdownAutomaton ParseAutomatonFile(const std::string& filename,
                                      AutomatonType type);
#endif 