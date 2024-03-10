#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <memory>
#include <stdexcept>
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
enum TOKEN_TYPES {
  ERROR,
  NUMBER,
  PRECEDENCE_1,
  PRECEDENCE_2,
  OPEN_PRN,
  CLOSE_PRN
};
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
  token tok;
  unique_ptr<node> left;
  unique_ptr<node> right;

  /* TO DO: am i really not allowed to use a constructor? remove unique_ptr use
   */
  node(token &t) : tok(t), left(nullptr), right(nullptr){};
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
 * Converts the string expression to its corresponding `token` structs.
 *
 * Throws an error on invalid token.
 *
 * @param expression The expression to be lexed (filtered for spaces)
 * @return the tokens for parsing
 * @throws invalid_argument Thrown when non digit, whitespace, or operator
 * character is found.
 */
vector<token> lexer(const string expr);

/**
 * Parses an expression based on the grammar rule:
 * expression : term { PRECEDENCE_1 term } .
 *
 * @param tokens The vector of tokens representing the input expression
 * @param currentToken Reference to the index of the current token being
 * processed
 * @return The result of the parsed expression.
 */
unique_ptr<node> parseExpression(const vector<token> &tokens, size_t &currentToken);

/**
 * Parses a term based on the grammar rule:
 * term : factor { PRECENDENCE_2 factor } .
 *
 * @param tokens The vector of tokens representing the input expression
 * @param currentToken Reference to the index of the current token being
 * processed
 * @return The result of the parsed term.
 */
unique_ptr<node> parseTerm(const vector<token> &tokens, size_t &currentToken);

/**
 * Parses an expression based on the grammar rule:
 * factor : NUMBER | OPEN_PRN expression CLOSE_PRN .
 *
 * @param tokens The vector of tokens representing the input expression
 * @param currentToken Reference to the index of the current token being
 * processed
 * @return The result of the parsed factor.
 *
 * @throws invalid_argument Thrown when mismatched parentheses are found or
 * consecutive operators are found.
 */
unique_ptr<node> parseFactor(const vector<token> &tokens, size_t &currentToken);

/**
 * Returns postfix expression as a string.
 *
 * @param root pointer to the root of the expression tree.
 * @return infix expression as a string (eg. "2 + 5")
 */
string displayTreeInfix(const unique_ptr<node> &root);

/**
 * Traverses expression tree inorder.
 *
 * @param root pointer to the root of the expression tree.
 * @param result empty string that gets concatenated with the tokens.
 */
void displayTreeInfixHelper(const unique_ptr<node> &root, string &result);

/**
 * Returns postfix expression as a string.
 *
 * @param root pointer to the root of the expression tree.
 * @return postfix expression as a string (eg. "2 5 +")
 */
string displayTreePostfix(const unique_ptr<node> &root);

/**
 * Traverses expression tree in postorder.
 *
 * @param root pointer to the root of the expression tree.
 * @param result empty string that gets concatenated with the tokens.
 */
void displayTreePostfixHelper(const unique_ptr<node> &root, string &result);

#endif // DEFINITIONS_H
