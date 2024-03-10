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

unique_ptr<node> parseExpression(const vector<token> &tokens, size_t &currentToken) {
  /* expression : term { PRECEDENCE_1 term } . */
  unique_ptr<node> currentTerm = parseTerm(tokens, currentToken);

  while (currentToken < tokens.size() &&
         tokens[currentToken].type == PRECEDENCE_1) {
    /* get the operation (+|-) */
    token op = tokens[currentToken++];

    unique_ptr<node> rightOperand = parseTerm(tokens, currentToken);
    unique_ptr<node> newNode = make_unique<node>();
    newNode->tok = op;
    newNode->left = std::move(currentTerm);
    newNode->right = std::move(rightOperand);
    currentTerm = std::move(newNode);
  }

  return currentTerm;
}

unique_ptr<node> parseTerm(const vector<token> &tokens, size_t &currentToken) {
  /* term : factor { PRECEDENCE_2 factor } . */
  unique_ptr<node> leftOperand = parseFactor(tokens, currentToken);

  while (currentToken < tokens.size() && tokens[currentToken].type == PRECEDENCE_2) {
    /* get the operation (*|/|%) */
    token op = tokens[currentToken++];

    unique_ptr<node> rightOperand = parseFactor(tokens, currentToken);
    unique_ptr<node> newNode = make_unique<node>();
    newNode->tok = op;
    newNode->left = std::move(leftOperand);
    newNode->right = std::move(rightOperand);
    leftOperand = std::move(newNode);
  }
  return leftOperand;
}

unique_ptr<node> parseFactor(const vector<token> &tokens, size_t &currentToken) {
  /* case: factor : NUMBER | OPEN_PRN expression CLOSE_PRN */
  if (currentToken < tokens.size() && tokens[currentToken].type == OPEN_PRN) {
    ++currentToken; /* consume OPEN_PRN token */

    unique_ptr<node> expression = parseExpression(tokens, currentToken);

    /* valid factor of case */
    if (currentToken < tokens.size() && tokens[currentToken].type == CLOSE_PRN) {
      ++currentToken; /* consume CLOSE_PRN */
      return expression;
    } else {
      /* mismatched parentheses */
      /* FIX: non exhaustive, works for */
      throw invalid_argument("Mismatched parentheses.");
    }
  }
  /* case: factor : NUMBER */
  else if (currentToken < tokens.size() && tokens[currentToken].type == NUMBER) {
    token numberToken = tokens[currentToken++];
    unique_ptr<node> numberPointer = make_unique<node>();
    numberPointer->tok = numberToken;
    return numberPointer;
  } else {
    /* handle invalid factor */
    throw invalid_argument("Cannot have two consecutive operators.");
  }
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

void isValidExpression(const unique_ptr<node> &root) {
  if (root) {
    isValidExpression(root->left);
    isValidExpression(root->right);
    /* all operators must have two children */
    if (root->tok.type == PRECEDENCE_1 or root->tok.type == PRECEDENCE_2)
      if (!root->left or !root->right)
        throw invalid_argument("All operators must have two children.");

    /* all operands must be leaves */
    else if (root->tok.type == NUMBER)
      if (root->left or root->right)
        throw invalid_argument("All operands must be leaves.");
  }
}
