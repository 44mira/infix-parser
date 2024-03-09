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
 * factor : NUMBER | "(" expression ")" .
 */

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

enum MENU_CHOICES { EXIT = 'X', PROG_DESC = 'P', EVALUATE = 'E' };
typedef int token_type;
typedef int type_data;
typedef char choice;

/**
 * `token` type that is what the lexer returns.
 *
 * these tokens correspond to terminal types in the grammar.
 */
enum TOKEN_TYPES { NUMBER, PRECEDENCE_1, PRECEDENCE_2 };
struct token {
  token_type type;
  string value;
};

/**
 * `node` type that can be either `Numeric `or `Operator`
 *
 * No methods because only limited to procedural code.
 *
 * `Numeric` types hold a value but no children nodes.
 * `Operator` types hold a value, and children nodes.
 */
enum NODE_TYPES { NUMERIC, OPERATOR };
struct node {
  type_data type;
  int value;
  node &left;
  node &right;
};

/**
 * Menu interface, returns a choice value based on what user inputs.
 *
 * @return menu choice
 */
choice menu(void);

/**
 * Prints out the program description to `stdout`.
 */
void programDescription(void);

/**
 * Enter the loop for evaluating expressions.
 * Exit back to `main` on 'X' or invalid expression.
 */
void evaluateLoop(void);

/**
 * Removes spaces from the input expression as the grammar deals by character.
 * Uses string to hold tokens as they're equivalent to arrays of characters.
 *
 * @param expression The expression to be lexed (filtered for spaces)
 * @ret the tokens for parsing
 */
string lexer(const string expression);

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
