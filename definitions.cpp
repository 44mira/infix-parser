#include "definitions.h"

vector<token> lexer(const string expr) {
  vector<token> tokens;
  for (int i = 0; i < expr.length(); i++) {
    if (isspace(expr[i]))
      continue;

    if (isdigit(expr[i])) {
      string number = "";

      // collect digits into a NUMBER token
      while (isdigit(expr[i])) {
        number += expr[i];
        i++;
      }
      tokens.push_back({NUMBER, number});
      i--;

      continue;
    }

    switch (expr[i]) {
    case '+':
    case '-':
      tokens.push_back({PRECEDENCE_1, string() + expr[i]});
      break;
    case '*':
    case '/':
    case '%':
      tokens.push_back({PRECEDENCE_2, string() + expr[i]});
      break;
    case '(':
      tokens.push_back({OPEN_PRN, "("});
      break;
    case ')':
      tokens.push_back({CLOSE_PRN, ")"});
      break;
    default:
      tokens.push_back({ERROR, ""});
    }
  }

  return tokens;
}
