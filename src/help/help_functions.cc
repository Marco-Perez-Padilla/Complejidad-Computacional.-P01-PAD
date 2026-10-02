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

** File help_functions.cc: functions that present messages to the user (help, usage, and non-exception warnings/errors).
**/

#include "help/help_functions.h"

#include <iostream>

/**
 * @brief Prints a short summary of the program and its options.
 */
void Help() {
  std::cout
      << "Pushdown automaton simulator\n\n"
      << "Usage: ./pda_simulator -config <file> -type <apv|apf> [-trace] "
         "[-in <file>] [-out <file>]\n\n"
      << "Options:\n"
      << "  -config <file>   Automaton definition file (required)\n"
      << "  -type <apv|apf>  Type of the automaton in that file (required)\n"
      << "  -trace           Show the simulation trace\n"
      << "  -in <file>       Input strings to check (optional; keyboard "
         "by default)\n"
      << "  -out <file>      File to write the trace to (optional; "
         "screen by default)\n"
      << "  --help, -h       Show this message and exit\n";
}

/**
 * @brief Prints a detailed description of the program, its command-line
 * options and the automaton definition file format. Called when the
 * arguments provided are incorrect.
 */
void Usage() {
  std::cout
      << "Pushdown automaton (PDA) simulator\n\n"
      << "Simulates a pushdown automaton, with acceptance by empty stack "
         "or by\n"
      << "final state. You must say which one with -type: the format of "
         "the\n"
      << "definition file is ambiguous when the set of final states is "
         "empty or\n"
      << "missing, so it cannot always be told apart from an empty-stack "
         "automaton\n"
      << "just by reading the file.\n\n"
      << "Command-line options:\n"
      << "  -config <file>   Automaton definition file (required)\n"
      << "  -type <apv|apf>  Type of the automaton in that file "
         "(required):\n"
      << "                    apv = acceptance by empty stack\n"
      << "                    apf = acceptance by final state\n"
      << "  -trace           Enable the simulation trace, showing the "
         "state, the\n"
      << "                    remaining input, the stack and the "
         "applicable\n"
      << "                    transitions after every step\n"
      << "  -in <file>       Input strings to check, one per line "
         "(optional;\n"
      << "                    without it, strings are read from the "
         "keyboard)\n"
      << "  -out <file>      File to write the trace to (optional; "
         "without it,\n"
      << "                    the trace is printed on screen)\n\n"
      << "Definition file format (comments start with '#'):\n"
      << "  q1 q2 q3 ...      set of states Q\n"
      << "  a1 a2 a3 ...      input alphabet Sigma\n"
      << "  A1 A2 A3 ...      stack alphabet Gamma\n"
      << "  q1                initial state\n"
      << "  A1                initial stack symbol\n"
      << "  q2 q3 ...         set of final states F (only when -type "
         "apf)\n"
      << "  q1 a A1 q2 A      one transition per line: "
         "delta(q1, a, A1) contains (q2, A)\n\n"
      << "Epsilon is written as a dot ('.'). On keyboard input, '.' "
         "denotes\n"
      << "the empty string and '..' terminates the program.\n\n"
      << "Example definition files are in data/automata/\n";
}

/**
 * @brief Prints a non-critical warning to stderr, with a uniform prefix.
 * A warning does not stop program execution.
 * @param message Description of the warning.
 */
void PrintWarning(const std::string& message) {
  std::cerr << "Warning: " << message << '\n';
}

/**
 * @brief Prints a critical error to stderr, with a uniform prefix. The
 * caller decides whether the program should terminate.
 * @param message Description of the error.
 */
void PrintError(const std::string& message) {
  std::cerr << "Error: " << message << '\n';
}

/**
 * @brief Parses and validates argv into options.
 * @param argc Argument count, as received by main.
 * @param argv Argument vector, as received by main.
 * @param options Filled in with the parsed options on success.
 * @return 0 if --help/-h was requested (Help() was already printed);
 *         -1 if the arguments are correct and options is ready to use;
 *          1 if the arguments are incorrect (an error and Usage() were
 *         already printed).
 */
int ValidateArguments(int argc, char* argv[], Options* options) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      Help();
      return 0;
    }
  }

  bool has_config = false;
  bool has_type = false;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-config") {
      if (i + 1 >= argc) {
        PrintError("-config requires a file name");
        Usage();
        return 1;
      }
      options->config_file = argv[++i];
      has_config = true;
    } else if (arg == "-type") {
      if (i + 1 >= argc) {
        PrintError("-type requires a value ('apv' or 'apf')");
        Usage();
        return 1;
      }
      std::string value = argv[++i];
      if (value == "apv") {
        options->type = AutomatonType::kEmptyStack;
      } else if (value == "apf") {
        options->type = AutomatonType::kFinalState;
      } else {
        PrintError("invalid value for -type: '" + value +
                    "' (expected 'apv' or 'apf')");
        Usage();
        return 1;
      }
      has_type = true;
    } else if (arg == "-trace") {
      options->trace = true;
    } else if (arg == "-in") {
      if (i + 1 >= argc) {
        PrintError("-in requires a file name");
        Usage();
        return 1;
      }
      options->input_file = argv[++i];
    } else if (arg == "-out") {
      if (i + 1 >= argc) {
        PrintError("-out requires a file name");
        Usage();
        return 1;
      }
      options->output_file = argv[++i];
    } else {
      PrintError("unknown option '" + arg + "'");
      Usage();
      return 1;
    }
  }

  if (!has_config) {
    PrintError("-config <file> is required");
    Usage();
    return 1;
  }
  if (!has_type) {
    PrintError("-type <apv|apf> is required");
    Usage();
    return 1;
  }
  if (options->output_file.has_value() && !options->trace) {
    PrintWarning("-out has no effect without -trace; ignoring it");
    options->output_file.reset();
  }
  return -1;
}