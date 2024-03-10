/**
 * Legolas Tyrael B. Lada | 2022-04734
 * Sharmaigne Angelie C. Mabano | 2022-03464
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
 * expression : OPEN_PRN term { PRECEDENCE_1 term } CLOSE_PRN.
 * term : factor { PRECENDENCE_2 factor } .
 * factor : NUMBER | expression .
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
      evaluationLoop();
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
  cin.ignore(1000, '\n');

  return toupper(ret); /* lowercase versions of P E X should be valid */
}

void programDescription(void) {
  cout << "\n\n\t\t==== PROGRAM DESCRIPTION ====\n\n"
          "Legolas Tyrael B. Lada       | 2022-04734\n"
          "Sharmaigne Angelie C. Mabano | 2022-03464\n"
          "March 05, 2024 - March 09, 2024\n=====\n"
          "Menu Descriptions:\n\n"
          "- [P] Program Description: Prints out the program description to "
          "stdout.\n"
          "- [E] Evaluate Expression(s): Takes an infix expression as input "
          "and displays its equivalent postfix expression and its result.\n"
          "- [X] Exit: Terminates the program.\n"
          "\n=====\n"
          "Work distribution:\n"
          "Tyrael: code structure setup, grammar, lexer, testing suite\n"
          "Sharmaigne: input validation, parser, evaluator\n\n"
          "\t\t============================\n\n";
}

void evaluationLoop(void) {
  string expr;
  choice exit;
  while (true) {
    try {
      cout << "\nInput an infix expression: ";
      getline(cin, expr);
      vector<token> tokens = lexer(expr);
      size_t currentToken = 0;
      unique_ptr<node> root = parseExpression(tokens, currentToken);

      if (currentToken != tokens.size()) 
        throw invalid_argument("Mismatch parentheses");
      cout << "\nPostfix expression: " << displayTreePostfix(root);
      cout << "\nResult of evaluation: ";
      
      try {
        cout << evaluatePostfix(root);
      } catch (invalid_argument& e) {
        cout << e.what();
      }

      cout << "\n\nDo you want to evaluate another expression?\n\n"
              "[X] NO\n"
              "[other] YES\n"
              "\nChoice: ";

      cin >> exit;
      cin.ignore(1000, '\n');
      if (toupper(exit) == EXIT)
        return;
    } catch(invalid_argument &e) {
      cout << "Invalid infix expression: ";
      cout << "\nPlease try again." << '\n';
    }
    
  }
}
