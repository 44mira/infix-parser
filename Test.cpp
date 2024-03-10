
#include "definitions.h"
#include <iostream>

using namespace std;

namespace Global {
  string tests_failed = "";
};

// Lexer unit tests
void lexer_tests();

// Parser unit tests
void parser_tests();

/**
 * Asserts an `expr` to be true. Throws an error `msg` otherwise.
 *
 * @param expr the expression to be tested
 * @param msg the error message to be thrown on false
 */
void assert(bool expr, string msg);

/**
 * Compares token vectors by element
 *
 * @param asserted the correct vector
 * @param tested the vector to be tested
 * @ret the result of the by-element comparions
 */
bool compare_tokens(vector<token> asserted, vector<token> tested);

int main(void) {

  cout << "\n";

  lexer_tests();
  parser_tests();

  if (!Global::tests_failed.empty())
    cout << "Tests failed:\n" << Global::tests_failed;
  else
    cout << "All tests passed!\n";

  cout << "\nTesting done.\n";

  return 0;
}

void assert(bool expr, string msg) {
  if (!expr)
     Global::tests_failed += "\nERROR: Test \"" + msg + "\" failed.\n";
}

bool compare_tokens(vector<token> asserted, vector<token> tested) {
  for (size_t i = 0; i < asserted.size(); i++) {
    if (asserted[i].type != tested[i].type ||
        asserted[i].value != tested[i].value)
      return false;
  }
  return true;
}

void lexer_tests() {
  vector<token> tested, asserted;

  asserted = {{NUMBER, "2"}};
  tested = lexer("2");
  assert(compare_tokens(asserted, tested),
         "Lexer can tokenize a single digit");

  asserted = {{NUMBER, "420"}};
  tested = lexer("420");
  assert(compare_tokens(asserted, tested),
         "Lexer can tokenize a multiple digit number");

  asserted = {{PRECEDENCE_1, "+"}};
  tested = lexer("+");
  assert(compare_tokens(asserted, tested),
         "Lexer can tokenize an operator");

  asserted = {{OPEN_PRN, "("}, {CLOSE_PRN, ")"}};
  tested = lexer("( )");
  assert(compare_tokens(asserted, tested),
         "Lexer can tokenize a parentheses and space");

  asserted = {{NUMBER, "34"}, {PRECEDENCE_1, "+"}, {NUMBER, "35"}};
  tested = lexer("34 + 35");
  assert(compare_tokens(asserted, tested),
         "Lexer can tokenize a basic expression");

  asserted = {{NUMBER, "1"},   {PRECEDENCE_1, "-"}, {OPEN_PRN, "("},
              {NUMBER, "5"},   {PRECEDENCE_2, "*"}, {NUMBER, "4"},
              {CLOSE_PRN, ")"}};
  tested = lexer("1-(5*4)");
  assert(compare_tokens(asserted, tested),
         "Lexer can tokenize a complex expression");
}

#define parse(expr) displayTreePostfix(parseExpression(lexer(expr), currentToken))
#define assert_this(expr) currentToken = 0; asserted = expr " "
#define assert_eq(msg) assert(asserted == tested, msg)
#define assert_block(asrt, tst, msg) {                            \
                                      try {                       \
                                        assert_this(asrt);        \
                                        tested = parse(tst);      \
                                        assert_eq(msg);           \
                                    } catch (...) {assert(false, msg);}}
#define assert_error(expr,msg) {                        \
                                try {                   \
                                  currentToken = 0;     \
                                  tested = parse(expr); \
                                  assert(false, msg);   \
                                } catch (...) {}}

void parser_tests() {
  string asserted, tested;
  size_t currentToken;

  assert_block("4", "4", "Parser can parse a single digit");

  assert_block("45", "45", "Parser can parse multiple digits");

  assert_block("34 35 +", "34 + 35", "Parser can parse a simple expression");

  assert_block("1 2 3 + *", "1 * (2 + 3)", "Parser can parse an expression with parentheses");

  assert_block("34 35 3 * +", "34 + 35 * 3", "Parser can parse expressions that require precedence");

  // assert_error("34 35 +", "Parser can catch invalid consecutive numbers");

}
