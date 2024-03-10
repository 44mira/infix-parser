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
      throw invalid_argument("Invalid token encountered.");
    }
  }

  return tokens;
}

unique_ptr<node> parseExpression(const vector<token>& tokens, size_t& currentToken) {
  /* expression : term { PRECEDENCE_1 term } . */
  unique_ptr<node> leftOperand = parseTerm(tokens, currentToken);

  while (currentToken < tokens.size() && tokens[currentToken].type == TOKEN_TYPES::PRECEDENCE_1) {
    /* get the operation (+|-) */
    token op = tokens[currentToken++];

    unique_ptr<node> rightOperand = parseTerm(tokens, currentToken);
    unique_ptr<node> newNode = make_unique<node>(op);
    newNode -> left = move(leftOperand);
    newNode -> right = move(rightOperand);
    leftOperand = move(newNode);
  }
  return leftOperand;
}

unique_ptr<node> parseTerm(const vector<token>& tokens, size_t& currentToken) {
  /* term : factor { PRECEDENCE_2 factor } . */
  unique_ptr<node> leftOperand = parseFactor(tokens, currentToken);

  while (currentToken < tokens.size() && tokens[currentToken].type == TOKEN_TYPES::PRECEDENCE_2) {
    /* get the operation (*|/|%) */
    token op = tokens[currentToken++];

    unique_ptr<node> rightOperand = parseFactor(tokens, currentToken);
    unique_ptr<node> newNode = make_unique<node>(op);
    newNode -> left = move(leftOperand);
    newNode -> right = move(rightOperand);
    leftOperand = move(newNode);
  }
  return leftOperand;
}

unique_ptr<node> parseFactor(const vector<token>& tokens, size_t& currentToken) {
  /* case: factor : OPEN_PRN expression CLOSE_PRN */
  if (currentToken < tokens.size() && tokens[currentToken].type == TOKEN_TYPES::OPEN_PRN) {
    ++currentToken; /* consume OPEN_PRN token */
    
    unique_ptr<node> expression = parseExpression(tokens, currentToken);

    /* valid factor of case */
    if (currentToken < tokens.size() && tokens[currentToken].type == TOKEN_TYPES::CLOSE_PRN) {
      ++currentToken; /* consume CLOSE_PRN */
      return expression;
    } else {
      /* mismatched parentheses */
      /* FIX: non exhaustive, works for */
      throw invalid_argument("Mismatched parentheses.");
    }
  } 
  /* case: factor : NUMBER */
  else if (currentToken < tokens.size() && tokens[currentToken].type == TOKEN_TYPES::NUMBER) {
    token numberToken = tokens[currentToken++];
    return make_unique<node>(numberToken);
  } else {
    /* handle invalid factor */
    throw invalid_argument("Cannot have two consecutive operators.");
  }
}

string displayTreeInfix(const unique_ptr<node>& root) {
  string result = "";
  displayTreeInfixHelper(root, result);
  return result;
}

void displayTreeInfixHelper(const unique_ptr<node>& root, string& result) {
  if (root) {
    displayTreeInfixHelper(root -> left, result);
    displayTreeInfixHelper(root -> right, result);
    result += root -> tok.value + " ";
  }
}
string displayTreePostfix(const unique_ptr<node>& root) {
  string result = "";
  displayTreePostfixHelper(root, result);
  return result;
}

void displayTreePostfixHelper(const unique_ptr<node>& root, string& result) {
  if (root) {
    displayTreePostfixHelper(root -> left, result);
    displayTreePostfixHelper(root -> right, result);
    result += root -> tok.value + " ";
  }
}