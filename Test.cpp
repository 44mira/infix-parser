
#include "definitions.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace Global {
  string tests_failed = "";
};

// Lexer unit tests
void lexerTests();

// Parser unit tests
void parserTests();

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
bool compareTokens(vector<token> asserted, vector<token> tested);

int main(void) {

  lexerTests();
  parserTests();

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

bool compareTokens(vector<token> asserted, vector<token> tested) {
  for (size_t i = 0; i < asserted.size(); i++) {
    if (asserted[i].type != tested[i].type ||
        asserted[i].value != tested[i].value)
      return false;
  }
  return true;
}

void lexerTests() {
  vector<token> tested, asserted;

  asserted = {{NUMBER, "2"}};
  tested = lexer("2");
  assert(compareTokens(asserted, tested),
         "Lexer can tokenize a single digit");

  asserted = {{NUMBER, "420"}};
  tested = lexer("420");
  assert(compareTokens(asserted, tested),
         "Lexer can tokenize a multiple digit number");

  asserted = {{PRECEDENCE_1, "+"}};
  tested = lexer("+");
  assert(compareTokens(asserted, tested),
         "Lexer can tokenize an operator");

  asserted = {{OPEN_PRN, "("}, {CLOSE_PRN, ")"}};
  tested = lexer("( )");
  assert(compareTokens(asserted, tested),
         "Lexer can tokenize a parentheses and space");

  asserted = {{NUMBER, "34"}, {PRECEDENCE_1, "+"}, {NUMBER, "35"}};
  tested = lexer("34 + 35");
  assert(compareTokens(asserted, tested),
         "Lexer can tokenize a basic expression");

  asserted = {{NUMBER, "1"},   {PRECEDENCE_1, "-"}, {OPEN_PRN, "("},
              {NUMBER, "5"},   {PRECEDENCE_2, "*"}, {NUMBER, "4"},
              {CLOSE_PRN, ")"}};
  tested = lexer("1-(5*4)");
  assert(compareTokens(asserted, tested),
         "Lexer can tokenize a complex expression");
}

/**
 *
 */
#define parse(expr) {                   \
                      currentToken = 0; \
                      tested = displayTreePostfix(parseExpression(lexer(expr), currentToken)); }

#define assert_this(expr) asserted = expr " "

#define assert_eq(msg) assert(asserted == tested, msg)

#define assert_block(asrt, tst, msg) {                                                          \
                                      try {                                                     \
                                        assert_this(asrt);                                      \
                                        parse(tst);                                             \
                                        assert_eq(msg);                                         \
                                    } catch (...) {assert(false, msg);}}
#define assert_error(err,expr,msg) {                                                                      \
                                try {                                                                     \
                                  parse(expr);                                                            \
                                  assert(false, msg);                                                     \
                                } catch (invalid_argument &e) {                                           \
                                  char buf[200];                                                          \
                                  sprintf(buf, "%s\n\t- EXPECTED: %s\n\t- GOT: %s", msg, err, e.what());  \
                                  assert(!strcmp(e.what(), err), buf);                                    \
                                }}

void parserTests() {
  string asserted, tested;
  size_t currentToken;

  assert_block("4",
               "4", "Parser can parse a single digit");

  assert_block("45", 
               "45", "Parser can parse multiple digits");

  assert_block("34 35 +", 
               "34 + 35", "Parser can parse a simple expression");

  assert_block("1 2 3 + *", 
               "1 * (2 + 3)", "Parser can parse an expression with parentheses");

  assert_block("34 35 3 * +", 
               "34 + 35 * 3", "Parser can parse expressions that require precedence");

  assert_block("5",
               "((5))", "Parser can disambiguate parentheses");

  assert_error("Cannot have consecutive numbers.",
               "34 35 +", "Parser can catch invalid consecutive numbers before operator.");

  assert_error("Cannot have consecutive numbers.",
               "34 / 31 32", "Parser can catch invalid consecutive numbers after operator.");

  assert_error("Stray operator found.",
               "+", "Parser can catch invalid stray operator.");

  assert_error("Mismatched parentheses.",
               "4 + 5 * ( 3 + 4", "Parser can catch invalid opening parenthesis.");

  assert_error("Mismatched parentheses.",
              "4 + 5 * 3 + 4 )", "Parser can catch invalid closing parenthesis.");

  assert_error("Cannot have two consecutive operators.",
               "4 + + 5", "Parser can catch invalid consecutive operators of same precedence.");

  assert_error("Cannot have two consecutive operators.",
               "4 + * 5", "Parser can catch invalid consecutive operators of differing precedence.");

  assert_error("Parentheses multiplication not supported.",
               "4 ( 5 + 3 )", "Parser can catch invalid number to parentheses.");

  assert_error("Parentheses multiplication not supported.",
               "(5 + 3) 4", "Parser can catch invalid parentheses to number.");
  
}
