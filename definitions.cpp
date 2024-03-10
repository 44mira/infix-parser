#include "definitions.h"
#include <memory>

using namespace std;

vector<token> lexer(const string expr) {
  vector<token> tokens;
  tokens.push_back({OPEN_PRN, "("});        // All expressions are enclosed by parentheses
  for (long unsigned i = 0; i < expr.length(); i++) {
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
  
  tokens.push_back({CLOSE_PRN, ")"});
  return tokens;
}

unique_ptr<node> parseExpression(const vector<token> &tokens, size_t &index) {
  
  if (tokens[index++].type != OPEN_PRN) throw invalid_argument("Mismatched parentheses.");

  // parse the current term
  unique_ptr<node> termNode = parseTerm(tokens, index);

  // parse the { PRECEDENCE_1 term }
  while (tokens[index].type == PRECEDENCE_1) {
    unique_ptr<node> ope = make_unique<node>();
    ope -> tok = tokens[index++];
    ope -> left = std::move(termNode);
    unique_ptr<node> nextTerm = parseTerm(tokens, index);
    ope -> right = std::move(nextTerm);
    
    termNode = std::move(ope);
  }

  if (tokens[index++].type == CLOSE_PRN)
    return termNode;
  throw invalid_argument("Mismatched parentheses.");
}

unique_ptr<node> parseTerm(const vector<token> &tokens, size_t &index) {
  
  // parse the current factor
  unique_ptr<node> factorNode = parseFactor(tokens, index);

  // parse the { PRECEDENCE_2 factor }
  while (tokens[index].type == PRECEDENCE_2) {
    unique_ptr<node> ope = make_unique<node>();
    ope -> tok = tokens[index++];
    ope -> left = std::move(factorNode);
    unique_ptr<node> nextTerm = parseFactor(tokens, index);
    ope -> right = std::move(nextTerm);

    factorNode = std::move(ope);
  }
  
  return factorNode;
}

unique_ptr<node> parseFactor(const vector<token> &tokens, size_t &index) {
  if (tokens[index].type == NUMBER) {
    unique_ptr<node> num = make_unique<node>();
    num -> tok = tokens[index++];
    return num;
  }

  return parseExpression(tokens, index);
}

string displayTreeInfix(const unique_ptr<node> &root) {
  string result = "";
  displayTreeInfixHelper(root, result);
  return result;
}

void displayTreeInfixHelper(const unique_ptr<node> &root, string &result) {
  if (root) {
    displayTreeInfixHelper(root->left, result);
    result += root->tok.value + " ";
    displayTreeInfixHelper(root->right, result);
  }
}

string displayTreePostfix(const unique_ptr<node> &root) {
  string result = "";
  vector<token> outputQueue;
  displayTreePostfixHelper(root, outputQueue);

  for (const token& tok : outputQueue) {
    result += tok.value + " ";
  }
  return result;
}

void displayTreePostfixHelper(const unique_ptr<node> &root, vector<token> &outputQueue) {
  if (root) {
    displayTreePostfixHelper(root->left, outputQueue);
    displayTreePostfixHelper(root->right, outputQueue);
    outputQueue.push_back(root->tok);
  }
}

double evaluatePostfix(const unique_ptr<node> &root) {
  vector<token> outputQueue;
  vector<double> operandStack;
  double firstOperand, secondOperand, opResult;

  displayTreePostfixHelper(root, outputQueue);

  for (const token& tok : outputQueue) {
    /* if operand, convert to double and push to operand stack */
    if (tok.type == NUMBER) {
      operandStack.push_back(stod(tok.value));
      continue;
    }

    /* else, it's an operator. evaluate */
    secondOperand = operandStack.back();
    operandStack.pop_back();
    firstOperand = operandStack.back();
    operandStack.pop_back();

    switch (tok.value[0]) {
    case '+': 
      opResult = firstOperand + secondOperand;
      break;
    case '-': 
      opResult = firstOperand - secondOperand; 
      break;
    case '*': 
      opResult = firstOperand * secondOperand; 
      break;
    case '/': 
      if (secondOperand == 0)
        throw invalid_argument("undefined");
        
      opResult = firstOperand / secondOperand; 
      break;
    /* modulo implementation for double data type */
    case '%': 
      if (secondOperand == 0)
        throw invalid_argument("undefined");
      opResult = firstOperand - floor(firstOperand/secondOperand) * secondOperand; 
      break;
    }

    /* push result back to operand stack */
    operandStack.push_back(opResult);
  }

  /* final result should be at the top of operand stack */
  return operandStack.back();
}
