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

** File menu.cc: implementation of Menu.
**/

#include "include/io/menu.h"

#include "include/exceptions/exceptions.h"
#include "include/help/help_functions.h"

namespace {
constexpr const char kExitCommand[] = "..";
}  // namespace

/**
  * @brief Builds a menu bound to a WordChecker and to the streams used for
  * interaction, so it can be exercised in tests with string streams
  * instead of std::cin/std::cout.
*/
Menu::Menu(const WordChecker& checker, std::istream& in, std::ostream& out)
    : checker_(checker), in_(in), out_(out) {}

/**
  * @brief Runs the interactive loop until the user chooses to exit or the input stream ends.
*/
void Menu::Run() {
  while (true) {
    out_ << "\n1) Enter strings from the keyboard\n"
         << "2) Check strings from a file\n"
         << "3) Exit\n"
         << "Choose an option: ";
    std::string choice;
    if (!std::getline(in_, choice)) return;
    if (choice == "1") {
      RunKeyboardMode();
    } else if (choice == "2") {
      RunFileMode();
    } else if (choice == "3") {
      return;
    } else {
      PrintWarning("invalid menu option '" + choice + "'");
    }
  }
}

/**
 * @brief Runs the interactive loop for checking words entered from the keyboard.
 * The user can enter the empty word as '.', and can exit this mode by entering
 * '..'.
 */
void Menu::RunKeyboardMode() {
  out_ << "Enter a string ('.' for the empty string, '..' to stop): ";
  std::string word;
  while (std::getline(in_, word)) {
    if (word == kExitCommand) break;
    checker_.Check(word);
    out_ << "Enter a string ('.' for the empty string, '..' to stop): ";
  }
}

/**
 * @brief Runs the interactive loop for checking words read from a file. If the
 * file cannot be opened, a warning is printed and the user is returned to the
 * main menu.
 */
void Menu::RunFileMode() {
  out_ << "Input file: ";
  std::string filename;
  if (!std::getline(in_, filename)) return;
  try {
    checker_.CheckAllFromFile(filename);
  } catch (const FileNotFoundException& e) {
    PrintWarning(e.what());
  }
}