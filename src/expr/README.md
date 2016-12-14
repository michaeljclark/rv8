# CParser • [ ![Build Status] [travis-image] ] [travis] [ ![License] [license-image] ] [license]

[travis-image]: https://travis-ci.org/cparse/cparse.png?branch=master
[travis]: http://travis-ci.org/cparse/cparse

[release-image]: http://img.shields.io/badge/release-0.2.1-blue.svg?style=flat
[releases]: https://github.com/cmusatyalab/openface/releases

[license-image]: http://img.shields.io/badge/license-MIT-blue.svg?style=flat
[license]: LICENSE.mit

This project provides a C++ library to parse a character sequence
as an expression using Dijkstra's
[Shunting-yard algorithm](http://en.wikipedia.org/wiki/Shunting-yard_algorithm),
which modifies
[Jesse Brown's original code](http://www.daniweb.com/software-development/cpp/code/427500/calculator-using-shunting-yard-algorithm).

*This project was developed by [Brandon Amos](http://bamos.github.io) and Vinícius Garcia.*

## Wiki

If you want to use this library in your project please take a look at our [Wiki](https://github.com/cparse/cparse/wiki)

---

# Modifications and Uses of This Code
+ In [Andrew Steiner's](https://github.com/awsteiner)
  [o2scl](https://github.com/awsteiner/o2scl):
  + Adds simple unary functions like sin, cos, and exp.
  + [shunting_yard.cpp](https://github.com/awsteiner/o2scl/blob/master/src/base/shunting_yard.cpp)
  + [shunting_yard.h](https://github.com/awsteiner/o2scl/blob/master/src/base/shunting_yard.h)
  + [shunting_yard_ts.cpp](https://github.com/awsteiner/o2scl/blob/master/src/base/shunting_yard_ts.cpp)

# Minimal examples.

## A simple calculator.

```C++
#include <iostream>
#include "shunting-yard.h"

int main() {
  TokenMap vars;
  vars["pi"] = 3.14;
  std::cout << calculator::calculate("-pi+1", &vars) << std::endl;

  // Or if you want to evaluate an expression
  // several times efficiently:
  calculator c1("pi-b");
  vars["b"] = 0.14;
  std::cout << c1.eval(vars) << std::endl; // 3
  vars["b"] = 2.14;
  std::cout << c1.eval(vars) << std::endl; // 1

  return 0;
}
```

## As a sub-parser for a programming language.

Here we implement an interpreter for multiple expressions, the delimiter used
will be `;` or `\n` just like Javascript or Python and the code must start and end on curly brackets.

A similar architecture can be used for interpreting other common programming language statements like `for` loops and `if` statements. If you're interested take a look on the [jSpy programming language](github.com/vingarcia/jspy) that uses this project as the core parsing system.

```C++
#include <iostream>
#include "shunting-yard.h"
#include "shunting-yard-exceptions.h"

struct codeBlock {
  static void interpret(const char* start, const char** end, TokenMap vars) {
    // Remove white spaces:
    while (isspace(*start)) ++start;

    if (*start != '{') {
      throw syntax_error("Expected '{'");
    } else {
      ++start;
    }

    while (*start != '}') {
      calculator::calculate(start, vars, ";\n}", &start);

      // Alternatively you could write above:
      // - calculator(start, ";\n}", &start).eval(vars);

      // Find the beginning of the next expression:
      while(isspace(*start) || *start == ';') ++start;
    }

    if (*start == '}') {
      *end = start+1;
    } else {
      throw syntax_error("Expected '}'");
    }
  }
};

int main() {
  GlobalScope vars;
  const char* code =
    "{"
    "  a = 10;"
    "  b = 20\n"
    "  c = a + b }";

  codeBlock::interpret(code, &code, vars);

  std::cout << vars["c"] << std::endl; // 30
  return 0;
}
```

Please note that a calculator can compile an expression so that it can efficiently be executed several times at a later moment.

## Using built-in List and Map.

```C++
int main() {
  GlobalScope vars;
  const char* code =
    "{"
    "  m = map(\n"
    "    'a':10,\n"
    "    'b':sqrt(4)*10\n"
    "  )\n"
    "  m.c = m['a'] + m[\"b\"]\n"

    // Note: A list can be built from a list of items,
    //       or from an iterable object such as a map():
    "  l = list(1,2,3) + list(m)\n"
    "}";

  // This class was described on the example above
  codeBlock::interpret(code, &code, vars);

  std::cout << vars["m"]["c"] << std::endl; // 30
  std::cout << vars["m"] << std::endl; // { "a": 10, "b": 20, "c": 30 }
  std::cout << vars["l"] << std::endl; // [ 1, 2, 3, "a", "b", "c" ]
  
  return 0;
}
```

## Easy definition of new built-in functions.

We describe two functions: `foo` and `bar`.

- `foo` works as a print() function.
- `bar` has no required arguments, but accept arguments anyway and prints them.

```C++
#include <iostream>
#include "shunting-yard.h"

const args_t args = {"str"};
packToken func_foo(TokenMap scope) {
  // Just print `str`
  std::string str = scope["str"].asString();
  std::cout << "foo " << str << std::endl;
  return packToken::None;
}

// Bar has no required arguments:
packToken func_bar(TokenMap scope) {
  std::cout << "arg-list: " << scope["args"] << std::endl;
  std::cout << "key-word: " << scope["kwargs"] << std::endl;
  return packToken::None;
}

// This initializer is a trick to run code before main() is executed:
struct FuncInitializer {
  FuncInitializer() {
    TokenMap& global_scope = TokenMap::default_global();

    // Register the functions on the default global scope:
    global_scope["bar"] = CppFunction(&func_bar, /*optional name:*/ "bar");
    global_scope["foo"] = CppFunction(&func_foo, args, "foo");
  }
} my_initializer;

int main() {
  GlobalScope vars;
  calculator::calculate("foo('10')"); // output: "foo 10"

  // Executing function bar with several arguments:
  calculator::calculate("bar('positional', 'args', 'key':'-', 'word':'args')");

  /*
   * Output:
   * arg-list: [ 'positional', 'args' ]
   * key-word: { 'key': '-', 'word': 'args' }
   */

  return 0;
}
```

## More examples.
 + See `test-shunting-yard.cpp`.

---

# Features.
 + Unary operators. +, -
 + Binary operators. +, -, /, *, %, <<, >>, ^
 + Boolean operators. <, >, <=, >=, ==, !=, &&, ||
 + Functions. sin, cos, tan, abs, print
 + Support for an hierarchy of scopes with local scope, global scope etc.
 + Easy to add new operators, operations, functions and even new types
 + Easy to implement object-to-object inheritance (with the prototype concept)
 + Built-in garbage collector (does not handle cyclic references yet)

# Writing your own operations.
To write your own operations:

 1. Copy the `operations.cpp` file.
 2. Edit it as you please; it is very easy to understand what to do.
 3. Compile your code and make sure to include your copied file instead of the original `.o`.

# Implementation Details
The main steps of the calculation process are:

 1. Create the operator precedence map.
 2. Convert to [RPN](http://en.wikipedia.org/wiki/Reverse_Polish_notation)
    with Dijkstra's Shunting-yard algorithm.
 3. Evaluate the expression in RPN form.

## Converting to RPN.
Most of the Shunting-yard algorithm resides here.
The idea is to do everything in one pass for elegance.
Please see the
[source code](https://github.com/bamos/cpp-expression-parser/blob/master/shunting-yard.cpp)
for implementation-specific details,
and refer to the pruned code below for a summary.

```C++
TokenQueue_t calculator::toRPN(const char* expr,
    std::map<std::string, double>* vars,
    std::map<std::string, int> opPrecedence) {
  TokenQueue_t rpnQueue; std::stack<std::string> operatorStack;

  while (*expr ) {
    if (isdigit(*expr )) {
      // If the token is a number, add it to the output queue.
    } else if (isvariablechar(*expr )) {
      // If the function is a variable, resolve it and
      // add the parsed number to the output queue.
    } else {
      // Otherwise, the variable is an operator or parenthesis.
      switch (*expr) {
        case '(':
          operatorStack.push("(");
          ++expr;
          break;
        case ')':
          while (operatorStack.top().compare("(")) {
            rpnQueue.push(new Token<std::string>(operatorStack.top()));
            operatorStack.pop();
          }
          operatorStack.pop();
          ++expr;
          break;
        default:
          {
            // The token is an operator.
            //
            // Let p(o) denote the precedence of an operator o.
            //
            // If the token is an operator, o1, then
            //   While there is an operator token, o2, at the top
            //       and p(o1) <= p(o2), then
            //     pop o2 off the stack onto the output queue.
            //   Push o1 on the stack.
          }
      }
    }
  }
  while (!operatorStack.empty()) {
    rpnQueue.push(new Token<std::string>(operatorStack.top()));
    operatorStack.pop();
  }
  return rpnQueue;
}
```


## Evaluating RPN form.
The RPN is represented as tokens in a stack.
To evaluate this, pop all of the elements off and handle
operations when encountered.


```C++
std::stack<double> evaluation;
while (!rpn.empty()) {
  TokenBase* base = rpn.front();
  rpn.pop();

  if (base->type == OP) {
    Token<std::string>* strTok = static_cast<Token<std::string>*>(base);
    std::string str = strTok->val;
    if (evaluation.size() < 2) {
      throw std::domain_error("Invalid equation.");
    }
    double right = evaluation.top(); evaluation.pop();
    double left  = evaluation.top(); evaluation.pop();
    if (!str.compare("+")) {
      evaluation.push(left + right);
    } else if (!str.compare("*")) {
      evaluation.push(left * right);
    } else if (!str.compare("-")) {
      evaluation.push(left - right);
    } else if (!str.compare("/")) {
      evaluation.push(left / right);
    } else if (!str.compare("<<")) {
      evaluation.push((int) left << (int) right);
    } else if (!str.compare(">>")) {
      evaluation.push((int) left >> (int) right);
    } else {
      throw std::domain_error("Unknown operator: '" + str + "'.");
    }
  } else if (base->type == NUM) {
    Token<double>* doubleTok = static_cast<Token<double>*>(base);
    evaluation.push(doubleTok->val);
  } else {
    throw std::domain_error("Invalid token.");
  }
  delete base;
}
```

The evaluated value resides in `evaluation.top` of type double.
