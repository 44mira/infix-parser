
#include "definitions.h"
#include <iostream>

using namespace std;

// Lexer unit tests
void lexer_tests();

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
  lexer_tests();

  return 0;
}

void lexer_tests() {
  vector<token> tested, asserted;

  try {
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
    assert(compare_tokens(asserted, tested), "Lexer can tokenize an operator");

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

  } catch (string msg) {
    cout << "ERROR: Test \"" << msg << "\" failed.\n";
  }
}

void assert(bool expr, string msg) {
  if (!expr)
    throw msg;
}

bool compare_tokens(vector<token> asserted, vector<token> tested) {
  for (size_t i = 0; i < asserted.size(); i++) {
    if (asserted[i].type != tested[i].type ||
        asserted[i].value != tested[i].value)
      return false;
  }
  return true;
}
