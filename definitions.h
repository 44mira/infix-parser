#include <string>
#include <vector>
#include <memory>

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
  token value;
  unique_ptr<node> left;
  unique_ptr<node> right;

  node(token t) : value(t), left(nullptr), right(nullptr) {};
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
 * @param expression The expression to be lexed (filtered for spaces)
 * @ret the tokens for parsing
 */
vector<token> lexer(const string expr);

/**
 * Parses an expression based on the grammar rule: 
 *  expression : term { PRECEDENCE_1 term } .
 * 
 * @param tokens The vector of tokens representing the input expression
 * @param currentToken Reference to the index of the current token being processed
 * @return The result of the parsed expression.
 */
unique_ptr<node> parseExpression(const vector<token>& tokens, size_t& currentToken);

/**
 * Parses an term based on the grammar rule: 
 * term : factor { PRECENDENCE_2 factor } .
 * 
 * @param tokens The vector of tokens representing the input expression
 * @param currentToken Reference to the index of the current token being processed
 * @return The result of the parsed term.
 */
unique_ptr<node> parseTerm(const vector<token>& tokens, size_t& currentToken);

/**
 * Parses an expression based on the grammar rule: 
 * factor : NUMBER | "(" expression ")" .
 * 
 * @param tokens The vector of tokens representing the input expression
 * @param currentToken Reference to the index of the current token being processed
 * @return The result of the parsed factor.
 */
unique_ptr<node> parseFactor(const vector<token>& tokens, size_t& currentToken);

/**
 * Print inorder traversal of the expression tree.
 * 
 * @param root pointer to the root of the expression tree.
 */
void displayTreeInfix(const unique_ptr<node>& root);

/**
 * Print postorder traversal of the expression tree.
 * 
 * @param root pointer to the root of the expression tree.
 */
void displayTreePostfix(const unique_ptr<node>& root);