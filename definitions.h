#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath> /* for floor */

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
void evaluationLoop(void);

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
 * Attempt to parse based on the following grammar rule:
 *
 * > expression : OPEN_PRN term { PRECEDENCE_1 term } CLOSE_PRN .
 *
 * @param tokens The tokens to be parsed
 * @param index The current index used in recursion
 * @return the expression node to be used in the AST
 * 
 * @throws `Invalid infix notation`
 */
unique_ptr<node> parseExpression(const vector<token> &tokens, size_t &index);

/**
 * Attempt to parse based on the following grammar rule:
 *
 * > term : factor { PRECEDENCE_2 factor } .
 *
 * @param tokens The tokens to be parsed
 * @param index The current index used in recursion
 * @return the expression node to be used in the AST
 * 
 * @throws `Invalid infix notation`
 */
unique_ptr<node> parseTerm(const vector<token> &tokens, size_t &index);

/**
 * Attempt to parse based on the following grammar rule:
 *
 * > factor : NUMBER | expression .
 *
 * @param tokens The tokens to be parsed
 * @param index The current index used in recursion
 * @return the expression node to be used in the AST
 * 
 * @throws `Invalid infix notation`
 */
unique_ptr<node> parseFactor(const vector<token> &tokens, size_t &index);

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
 * @param outputQueue reference to vector that gets pushed with the tokens.
 */
void displayTreePostfixHelper(const unique_ptr<node> &root, vector<token> &outputQueue);

/**
 * Evaluates expression tree by postfix.
 * 
 * @param root pointer to the root of the expression tree.
 * @return result of the evaluation
 * 
 * @throws invalid_argument thrown when attempting to divide by 0 (in / and %)
 */
double evaluatePostfix(const unique_ptr<node> &root);

#endif // DEFINITIONS_H
