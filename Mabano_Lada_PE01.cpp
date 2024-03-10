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
#include <memory>

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
          "\nChoice: ";
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
          "Tyrael: code structure setup, grammar, lexer, testing suite\n"
          "Sharmaigne: input validation, parser, evaluator\n\n"
          "\t\t============================\n\n";
}

void evaluateLoop(void) {
  string expr;
  choice exit;
  while (true) {
    try {
      cout << "\nInput an infix expression: ";
      cin.ignore();
      getline(cin, expr);
      size_t currentToken = 0;
      unique_ptr<node> root = parseExpression(lexer(expr), currentToken);

      cout << "Postfix expression: ";
      displayTreePostfix(root);

      cout << "\n\nDo you want to evaluate another expression?\n\n"
              "[X] NO\n"
              "[other] YES\n"
              "\nChoice: ";

      cin >> exit;
      if (toupper(exit) == EXIT)
        return;
    } catch(invalid_argument& e) {
      cout << "Invalid Input: ";
      cerr << e.what();
      cout << "\nPlease try again." << '\n';
    }
    
  }
}

void displayTreeInfix(const unique_ptr<node>& root) {
  if (root) {
    displayTreeInfix(root -> left);
    cout << root -> tok.value << " ";
    displayTreeInfix(root -> right);
  }
}

void displayTreePostfix(const unique_ptr<node>& root) {
  if (root) {
    displayTreePostfix(root -> left);
    displayTreePostfix(root -> right);
    cout << root -> tok.value << " ";
  }
}