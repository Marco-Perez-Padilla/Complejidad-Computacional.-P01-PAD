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

** File automaton_parser.cc: reads a pushdown automaton definition from a text file.
**/

#include "include/io/automaton_parser.h"

#include <fstream>
#include <set>
#include <sstream>
#include <vector>

#include "include/exceptions/exceptions.h"
#include "include/help/help_functions.h"
#include "include/model/automaton_type.h"

namespace {

struct RawLine {
  int number;
  std::string content;
};

/**
 * @brief Removes a trailing '#' comment from line and trims the
 * surrounding whitespace.
 */
std::string StripComment(const std::string& line) {
  std::string result = line.substr(0, line.find('#'));
  size_t begin = result.find_first_not_of(" \t\r");
  if (begin == std::string::npos) return "";
  size_t end = result.find_last_not_of(" \t\r");
  return result.substr(begin, end - begin + 1);
}

/**
 * @brief Splits line into whitespace-separated tokens.
 */
std::vector<std::string> Tokenize(const std::string& line) {
  std::istringstream stream(line);
  std::vector<std::string> tokens;
  std::string token;
  while (stream >> token) tokens.push_back(token);
  return tokens;
}

/**
 * @brief Reads filename and returns every line with content, after
 * stripping comments, together with its original line number.
 */
std::vector<RawLine> ReadMeaningfulLines(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) throw FileNotFoundException(filename);

  std::vector<RawLine> lines;
  std::string raw;
  int number = 0;
  while (std::getline(file, raw)) {
    ++number;
    std::string content = StripComment(raw);
    if (!content.empty()) lines.push_back({number, content});
  }
  if (lines.empty()) throw EmptyFileException(filename);
  return lines;
}

/**
 * @brief Parses the tokens of line as an Alphabet (Sigma or Gamma).
 */
Alphabet ParseAlphabet(const RawLine& line, const std::string& alphabet_name) {
  Alphabet alphabet;
  for (const std::string& token : Tokenize(line.content)) {
    if (token.size() != 1) {
      throw InvalidDefinitionException(
          line.number, alphabet_name + " symbols must be a single character: '" + token + "'");
    }
    if (!alphabet.AddSymbol(token[0])) {
      throw InvalidDefinitionException(
          line.number, "'" + token + "' is reserved and cannot belong to " + alphabet_name);
    }
  }
  return alphabet;
}

/**
 * @brief Parses the tokens of line as a set of state names.
 */
std::set<std::string> ParseStateSet(const RawLine& line) {
  std::set<std::string> states;
  for (const std::string& token : Tokenize(line.content)) states.insert(token);
  return states;
}

/**
 * @brief Checks whether tokens has the shape of a well-formed
 * transition line given the sets already known.
 */
bool LooksLikeTransition(const std::vector<std::string>& tokens,
                          const std::set<std::string>& states,
                          const Alphabet& input_alphabet,
                          const Alphabet& stack_alphabet) {
  if (tokens.size() != 5) return false;
  if (states.find(tokens[0]) == states.end()) return false;

  bool valid_input = tokens[1] == "." || (tokens[1].size() == 1 && input_alphabet.Contains(tokens[1][0]));
  if (!valid_input) return false;

  if (tokens[2].size() != 1 || !stack_alphabet.Contains(tokens[2][0])) {return false;}
  return states.find(tokens[3]) != states.end();
}

/**
 * @brief Parses line as a single transition.
 */
Transition ParseTransition(const RawLine& line,
                            const std::set<std::string>& states,
                            const Alphabet& input_alphabet,
                            const Alphabet& stack_alphabet) {
  std::vector<std::string> tokens = Tokenize(line.content);
  if (tokens.size() != 5) {
    throw InvalidDefinitionException(
        line.number, "a transition must have exactly 5 fields");
  }

  Transition transition;
  transition.from = tokens[0];
  if (states.find(transition.from) == states.end()) {
    throw InvalidDefinitionException(
        line.number, "state '" + transition.from + "' is not in Q");
  }

  if (tokens[1] == ".") {
    transition.input = kEpsilon;
  } else if (tokens[1].size() == 1 && input_alphabet.Contains(tokens[1][0])) {
    transition.input = tokens[1][0];
  } else {
    throw InvalidDefinitionException(
        line.number,
        "'" + tokens[1] + "' is not in Sigma nor the epsilon marker '.'");
  }

  if (tokens[2].size() != 1 || !stack_alphabet.Contains(tokens[2][0])) {
    throw InvalidDefinitionException(
        line.number, "'" + tokens[2] + "' is not in Gamma");
  }
  transition.top = tokens[2][0];

  transition.to = tokens[3];
  if (states.find(transition.to) == states.end()) {
    throw InvalidDefinitionException(
        line.number, "state '" + transition.to + "' is not in Q");
  }

  if (tokens[4] == ".") {
    transition.push = "";
  } else {
    for (char symbol : tokens[4]) {
      if (!stack_alphabet.Contains(symbol)) {
        throw InvalidDefinitionException(
            line.number, "'" + std::string(1, symbol) + "' is not in Gamma");
      }
    }
    transition.push = tokens[4];
  }
  return transition;
}

/**
 * @brief Checks whether transition already exists in table with the same destination and push string.
 */
bool IsDuplicate(const TransitionTable& table, const Transition& transition) {
  for (const Transition& existing : table.Find(transition.from, transition.input, transition.top)) {
    if (existing.to == transition.to && existing.push == transition.push) {
      return true;
    }
  }
  return false;
}

}  // namespace

/**
 * @brief Reads a pushdown automaton definition from filename and builds
 * the corresponding PushdownAutomaton.
 *
 * The automaton type (empty stack or final state) is detected
 * automatically: the sixth meaningful line is treated as the set of
 * final states if every one of its tokens is a declared state and it
 * does not look like a well-formed transition; otherwise it is treated
 * as the first transition of an empty-stack automaton.
 *
 * @param filename Path to the definition file.
 * @return The parsed, validated automaton.
 * @throws FileNotFoundException if filename cannot be opened.
 * @throws EmptyFileException if the file has no meaningful content.
 * @throws InvalidDefinitionException if the definition is malformed or violates the formal definition of a pushdown automaton.
 */
PushdownAutomaton ParseAutomatonFile(const std::string& filename) {
  std::vector<RawLine> lines = ReadMeaningfulLines(filename);
  if (lines.size() < 5) {
    throw InvalidDefinitionException(
        lines.back().number,
        "the definition must include Q, Sigma, Gamma, the initial state and the initial stack symbol");
  }

  size_t index = 0;
  std::set<std::string> states = ParseStateSet(lines[index]);
  if (states.empty()) {
    throw InvalidDefinitionException(lines[index].number, "Q must not be empty");
  }
  ++index;

  Alphabet input_alphabet = ParseAlphabet(lines[index++], "Sigma");
  Alphabet stack_alphabet = ParseAlphabet(lines[index++], "Gamma");

  std::vector<std::string> initial_state_tokens = Tokenize(lines[index].content);
  if (initial_state_tokens.size() != 1) {
    throw InvalidDefinitionException(
        lines[index].number, "the initial state must be a single token");
  }
  std::string initial_state = initial_state_tokens[0];
  if (states.find(initial_state) == states.end()) {
    throw InvalidDefinitionException(
        lines[index].number, "initial state '" + initial_state + "' is not in Q");
  }
  ++index;

  std::vector<std::string> initial_stack_tokens = Tokenize(lines[index].content);
  if (initial_stack_tokens.size() != 1 || initial_stack_tokens[0].size() != 1) {
    throw InvalidDefinitionException(
        lines[index].number, "the initial stack symbol must be a single character");
  }
  char initial_stack_symbol = initial_stack_tokens[0][0];
  if (!stack_alphabet.Contains(initial_stack_symbol)) {
    throw InvalidDefinitionException(
        lines[index].number, "initial stack symbol '" + std::string(1, initial_stack_symbol) + "' is not in Gamma");
  }
  ++index;

  if (index >= lines.size()) {
    throw InvalidDefinitionException(lines.back().number, "the definition has no transitions");
  }

  AutomatonType type;
  std::set<std::string> final_states;
  TransitionTable transitions;

  std::vector<std::string> next_tokens = Tokenize(lines[index].content);
  if (LooksLikeTransition(next_tokens, states, input_alphabet, stack_alphabet)) {
    type = AutomatonType::kEmptyStack;
    transitions.Add(
        ParseTransition(lines[index], states, input_alphabet, stack_alphabet));
    ++index;
  } else {
    bool all_states = !next_tokens.empty();
    for (const std::string& token : next_tokens) {
      if (states.find(token) == states.end()) {
        all_states = false;
        break;
      }
    }
    if (!all_states) {
      throw InvalidDefinitionException(
          lines[index].number,
          "line is neither a valid set of final states nor a well-formed transition");
    }
    type = AutomatonType::kFinalState;
    for (const std::string& token : next_tokens) final_states.insert(token);
    ++index;
  }

  for (; index < lines.size(); ++index) {
    Transition transition =
        ParseTransition(lines[index], states, input_alphabet, stack_alphabet);
    if (IsDuplicate(transitions, transition)) {
      PrintWarning("line " + std::to_string(lines[index].number) + ": duplicate transition ignored");
      continue;
    }
    transitions.Add(transition);
  }

  return PushdownAutomaton(states, input_alphabet, stack_alphabet,
                            initial_state, initial_stack_symbol, transitions,
                            type, final_states);
}