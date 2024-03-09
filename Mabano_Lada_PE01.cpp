/**
 * Legolas Tyrael B. Lada | 2022-04734
 * 2-BSCS
 *
 * An infix to postfix evaluator using a parse tree.
 */

/**
 * GRAMMAR (modified EBNF)
 * - pipes `|` correspond to alternatives
 * - parentheses `( )` symbolize grouped tokens
 * - braces `{ }` mean 0 or more
 * - brackets `[ ]` mean 0 or 1
 * - colon `:` means definition
 * - quoted tokens `" "` is a terminal definition
 * - dot `.` means the end of a definition
 *
 * expression : term { ("+" | "-") term } .
 * term : factor { ("*" | "/" | "%") factor } .
 * factor : numeric | "(" expression ")" .
 * digit : "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" .
 * numeric : { digit } (digit | "0") .
 */

#include <iostream>
#include <vector>

enum TYPES { Numeric, Operator };
typedef int type_data;

/**
 * `node` type that can be either `Numeric `or `Operator`
 *
 * No methods because only limited to procedural code.
 *
 * `Numeric` types hold a value but not a left and right node.
 * `Operator` types hold a left and right node but not a value.
 */
struct node {
  type_data type;
  int value;
  node &left;
  node &right;
};

int main(void) {}
