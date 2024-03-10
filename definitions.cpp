#include "definitions.h"

using namespace std;

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

/* TO DO: WRITE PARSER FUNCTIONS DEFINITIONS HERE */


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