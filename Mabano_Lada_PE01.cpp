/**
 * Legolas Tyrael B. Lada | 2022-04734
 * 2-BSCS
 *
 * An infix to postfix evaluator using a parse tree.
 */

/**
 * GRAMMAR (modified EBNF)
 * - pipes `|` correspond to alternatives
 * - parentheses `( )` symbolize grouped tokens
 * - braces `{ }` mean 0 or more
 * - brackets `[ ]` mean 0 or 1
 * - colon `:` means definition
 * - quoted tokens `" "` and CAPITALIZED are terminal definitions
 * - dot `.` means the end of a definition
 *
 * expression : term { PRECEDENCE_1 term } .
 * term : factor { PRECENDENCE_2 factor } .
 * factor : NUMBER | OPEN_PRN expression CLOSE_PRN .
 */

#include "definitions.h"
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

int main(void) {
  bool running = true;

  while (running) {
    switch (menu()) {
    case PROG_DESC:
      programDescription();
      break;
    case EVALUATE:
      evaluateLoop();
      break;
    case EXIT:
      running = false;
      break;
    default:
      cout << "\nInvalid input, try again.\n";
    }
  }
  return 0;
}

choice menu(void) {
  choice ret;
  cout << "\nWelcome to this Expression Evaluator program! Please choose an "
          "action to perform...\n"
          "[P] Program Description\n"
          "[E] Evaluate Expression(s)\n"
          "[X] Exit\n"
          "Choice: ";
  cin >> ret;

  return toupper(ret); // lowercase versions of P E X should be valid
}

void programDescription(void) {
  cout << "\n\n\t\t==== PROGRAM DESCRIPTION ====\n\n"
          "Legolas Tyrael B. Lada       | 2022-04734\n"
          "Sharmaigne Angelie C. Mabano | 2022-03464\n"
          "March 05, 2024 - March 09, 2024\n=====\nMenu Descriptions:\n\n"

          "- [P] Program Description: Prints out the program description to "
          "stdout.\n"
          "- [E] Evaluate Expression(s): Takes an infix expression as input "
          "and displays its equivalent postfix expression and its result.\n"
          "- [X] Exit: Terminates the program.\n\n=====\n"

          "Work distribution:\n"
          "Tyrael: Code structure setup, infix to postfix conversion\n"
          "Sharmaigne: Input validation, postfix expression evaluation\n\n"
          "\t\t============================\n\n";
}

void evaluateLoop(void) {
  string expr;
  choice exit;
  while (true) {
    cout << "\n Input an infix expression: ";
    cin.ignore();
    getline(cin, expr);

    cout << "\nDo you want to evaluate another expression?\n\n"
            "[X] NO\n"
            "[other] YES\n"
            "Choice: ";

    cin >> exit;
    if (toupper(exit) == EXIT)
      return;
  }
}

vector<token> lexer(const string expr) {
  vector<token> tokens;
  for (int i = 0; i < expr.length(); i++) {

    if (isdigit(expr[i])) {
      string number = "";

      // collect digits into a NUMBER token
      while (isdigit(expr[i])) {
        number += expr[i];
        i++;
      }
      tokens.push_back({NUMBER, number});
      i--;

      continue;
    }

    switch (expr[i]) {
    case '+':
    case '-':
      tokens.push_back({PRECEDENCE_1, string() + expr[i]});
      break;
    case '*':
    case '/':
    case '%':
      tokens.push_back({PRECEDENCE_2, string() + expr[i]});
      break;
    case '(':
      tokens.push_back({OPEN_PRN, "("});
      break;
    case ')':
      tokens.push_back({CLOSE_PRN, ")"});
      break;
    default:
      tokens.push_back({ERROR, ""});
    }
  }

  return tokens;
}
