
#include "definitions.h"
#include <iostream>
#include <stdexcept>

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

#define parse(expr) tested = displayTreePostfix(parseExpression(lexer(expr), currentToken))
#define assert_block(block) try {}
#define assert_this(expr) currentToken = 0; asserted = expr " "
#define assert_eq(msg) assert(asserted == tested, msg)

void parser_tests() {
  string asserted, tested;
  size_t currentToken;

  assert_this("4");
  parse("4");
  assert_eq("Parser can parse a single digit");

  assert_this("45");
  parse("45");
  assert_eq("Parser can parse multiple digits");

  assert_this("34 35 +");
  parse("34 + 35");
  assert_eq("Parser can parse a simple expression");

  // assert_this("1 - ( 5 * 4 )");
  // parse("1 - ( 5 + 4 )");
  // assert_eq("Parser can parse expressions with parentheses");

  // parse("34 + 35 * 3");

}
