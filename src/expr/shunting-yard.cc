#include "./shunting-yard.h"
#include "./shunting-yard-exceptions.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <string>
#include <stack>
#include <utility>  // For std::pair
#include <cstring>  // For strchr()

/* * * * * Operation class: * * * * */

// Convert a type into an unique mask for bit wise operations:
const uint32_t Operation::mask(tokType_t type) {
  if (type == ANY_TYPE) {
    return 0xFFFF;
  } else {
    return ((type & 0xE0) << 24) | (1 << (type & 0x1F));
  }
}

// Build a mask for each pair of operands
const opID_t Operation::build_mask(tokType_t left, tokType_t right) {
  opID_t result = mask(left);
  return (result << 32) | mask(right);
}

/* * * * * Utility functions: * * * * */

bool match_op_id(opID_t id, opID_t mask) {
  uint64_t result = id & mask;
  uint32_t* val = reinterpret_cast<uint32_t*>(&result);
  if (val[0] && val[1]) return true;
  return false;
}

TokenBase* exec_operation(const packToken& left, const packToken& right,
                          evaluationData* data, const std::string& OP_MASK) {
  auto it = data->opMap.find(OP_MASK);
  if (it == data->opMap.end()) return 0;
  for (const Operation& operation : it->second) {
    if (match_op_id(data->opID, operation.getMask())) {
      try {
        return operation.exec(left, right, data).release();
      } catch (Operation::Reject e) {
        continue;
      }
    }
  }

  return 0;
}

// Use this function to discard a reference to an object
// And obtain the original TokenBase*.
// Please note that it only deletes memory if the token
// is of type REF.
TokenBase* resolve_reference(TokenBase* b, TokenMap* scope = 0) {
  TokenBase* value = 0;

  if (b->type & REF) {
    // Grab the possible values:
    RefToken* ref = static_cast<RefToken*>(b);

    // If its a local variable,
    // and a local scope is available:
    if (ref->source->type == NONE && scope) {
      // Try to get the most recent value of the reference:
      packToken* r_value = scope->find(ref->key.asString());
      if (r_value) {
        value = (*r_value)->clone();
      }
    }

    if (!value) value = std::move(ref->value).release();
    delete ref;

    return value;
  } else {
    return b;
  }
}

/* * * * * Static containers: * * * * */

// Builds the opPrecedence map only once:
OppMap_t& calculator::default_opPrecedence() {
  static OppMap_t opp;
  return opp;
}

typeMap_t& calculator::type_attribute_map() {
  static typeMap_t type_map;
  return type_map;
}

opMap_t& calculator::default_opMap() {
  static opMap_t opMap;
  return opMap;
}

rWordMap_t& calculator::default_rWordMap() {
  static rWordMap_t rwMap;
  return rwMap;
}

/* * * * * rpnBuilder Class: * * * * */

void rpnBuilder::cleanRPN(TokenQueue_t* rpn) {
  while (rpn->size()) {
    delete resolve_reference(rpn->front());
    rpn->pop();
  }
}

// Check for unary operators and "convert" them to binary:
void rpnBuilder::handle_unary(const std::string& op) {
  if (this->lastTokenWasOp) {
    // Convert unary operators to binary in the RPN.
    if (!op.compare("-") || !op.compare("+")) {
      this->rpn.push(new Token<int64_t>(0, INT));
      this->lastTokenWasUnary = true;
    } else {
      cleanRPN(&(this->rpn));
      throw std::domain_error("Unrecognized unary operator: '" + op + "'.");
    }
  } else {
    this->lastTokenWasUnary = false;
  }
}

// Consume operators with precedence >= than op then add op
void rpnBuilder::handle_op(const std::string& op) {
  // "Convert" unary operators into binary, so they can
  // be treated as if they were the same:
  handle_unary(op);

  // Check if operator exists:
  if (!opp.exists(op)) {
    cleanRPN(&(rpn));
    throw std::domain_error("Undefined operator: `" + op + "`!");
  }

  // Let p(o) denote the precedence of an operator o.
  //
  // If the token is an operator, o1, then
  //   While there is an operator token, o2, at the top
  //       and p(o1) >= p(o2), then (`>` for Right to Left associativity)
  //     pop o2 off the stack onto the output queue.
  //   Push o1 on the stack.

  // If it associates from left to right:
  if (opp.assoc(op) == 0) {
    while (!opStack.empty() && opp.prec(op) >= opp.prec(opStack.top())) {
      rpn.push(new Token<std::string>(opStack.top(), OP));
      opStack.pop();
    }
  } else {
    while (!opStack.empty() && opp.prec(op) > opp.prec(opStack.top())) {
      rpn.push(new Token<std::string>(opStack.top(), OP));
      opStack.pop();
    }
  }
  opStack.push(op);

  lastTokenWasOp = op[0];
}

void rpnBuilder::handle_token(TokenBase* token) {
  rpn.push(token);
  lastTokenWasOp = false;
  lastTokenWasUnary = false;
}

void rpnBuilder::open_bracket(const std::string& bracket) {
  opStack.push(bracket);
  lastTokenWasOp = bracket[0];
  lastTokenWasUnary = false;
  ++bracketLevel;
}

void rpnBuilder::close_bracket(const std::string& bracket) {
  if (lastTokenWasOp == bracket[0]) {
    rpn.push(new Tuple());
  }
  while (opStack.size() && opStack.top() != bracket) {
    rpn.push(new Token<std::string>(opStack.top(), OP));
    opStack.pop();
  }

  if (opStack.size() == 0) {
    rpnBuilder::cleanRPN(&rpn);
    throw syntax_error("Extra '" + bracket + "' on the expression!");
  }

  opStack.pop();
  lastTokenWasOp = false;
  lastTokenWasUnary = false;
  --bracketLevel;
}

/* * * * * RAII_TokenQueue_t struct  * * * * */

// Used to make sure an rpn is dealloc'd correctly
// even when an exception is thrown.
//
// Note: This is needed because C++ does not
// allow a try-finally block.
struct calculator::RAII_TokenQueue_t : TokenQueue_t {
  RAII_TokenQueue_t() {}
  RAII_TokenQueue_t(const TokenQueue_t& rpn) : TokenQueue_t(rpn) {}
  ~RAII_TokenQueue_t() { rpnBuilder::cleanRPN(this); }

  RAII_TokenQueue_t(const RAII_TokenQueue_t& rpn) {
    throw std::runtime_error("You should not copy this class!");
  }
  RAII_TokenQueue_t& operator=(const RAII_TokenQueue_t& rpn) {
    throw std::runtime_error("You should not copy this class!");
  }
};

/* * * * * calculator class * * * * */

#define isvariablechar(c) (isalpha(c) || c == '_')
TokenQueue_t calculator::toRPN(const char* expr,
                               TokenMap vars, const char* delim,
                               const char** rest, OppMap_t opPrecedence,
                               rWordMap_t rWordMap) {
  rpnBuilder data(vars, opPrecedence);
  char* nextChar;

  static char c = '\0';
  if (!delim) delim = &c;

  while (*expr && isspace(*expr) && !strchr(delim, *expr)) ++expr;

  if (*expr == '\0' || strchr(delim, *expr)) {
    throw std::invalid_argument("Cannot build a calculator from an empty expression!");
  }

  // In one pass, ignore whitespace and parse the expression into RPN
  // using Dijkstra's Shunting-yard algorithm.
  while (*expr && (data.bracketLevel || !strchr(delim, *expr))) {
    if (isdigit(*expr)) {
      // If the token is a number, add it to the output queue.
      int64_t _int = strtol(expr, &nextChar, 10);

      // If the number was not a float:
      if (!strchr(".eE", *nextChar)) {
        data.handle_token(new Token<int64_t>(_int, INT));
      } else {
        double digit = strtod(expr, &nextChar);
        data.handle_token(new Token<double>(digit, REAL));
      }

      expr = nextChar;
    } else if (isvariablechar(*expr)) {
      rWordMap_t::iterator it;

      // If the token is a variable, resolve it and
      // add the parsed number to the output queue.
      std::stringstream ss;
      ss << *expr;
      ++expr;
      while (isvariablechar(*expr) || isdigit(*expr)) {
        ss << *expr;
        ++expr;
      }
      std::string key = ss.str();

      if (data.lastTokenWasOp == '.') {
        data.handle_token(new Token<std::string>(key, STR));
      } else if ((it=rWordMap.find(key)) != rWordMap.end()) {
        // Parse reserved words:
        try {
          it->second(expr, &expr, &data);
        } catch (...) {
          rpnBuilder::cleanRPN(&data.rpn);
          throw;
        }
      } else {
        packToken* value = vars.find(key);

        if (value) {
          // Save a reference token:
          TokenBase* copy = (*value)->clone();
          data.handle_token(new RefToken(key, copy));
        } else {
          // Save the variable name:
          data.handle_token(new Token<std::string>(key, VAR));
        }
      }
    } else if (*expr == '\'' || *expr == '"') {
      // If it is a string literal, parse it and
      // add to the output queue.
      char quote = *expr;

      ++expr;
      std::stringstream ss;
      while (*expr && *expr != quote && *expr != '\n') {
        if (*expr == '\\') {
          switch (expr[1]) {
          case 'n':
            expr+=2;
            ss << '\n';
            break;
          case 't':
            expr+=2;
            ss << '\t';
            break;
          default:
            if (strchr("\"'\n", expr[1])) ++expr;
            ss << *expr;
            ++expr;
          }
        } else {
          ss << *expr;
          ++expr;
        }
      }

      if (*expr != quote) {
        std::string squote = (quote == '"' ? "\"": "'");
        rpnBuilder::cleanRPN(&data.rpn);
        throw syntax_error("Expected quote (" + squote +
                           ") at end of string declaration: " + squote + ss.str() + ".");
      }
      ++expr;
      data.handle_token(new Token<std::string>(ss.str(), STR));
    } else {
      // Otherwise, the variable is an operator or paranthesis.

      // Check for syntax errors (excess of operators i.e. 10 + + -1):
      if (data.lastTokenWasUnary) {
        std::string op;
        op.push_back(*expr);
        rpnBuilder::cleanRPN(&data.rpn);
        throw syntax_error("Expected operand after unary operator `" + data.opStack.top() +
                           "` but found: `" + op + "` instead.");
      }

      switch (*expr) {
      case '(':
        // If it is a function call:
        if (data.lastTokenWasOp == false) {
          // This counts as a bracket and as an operator:
          data.handle_op("()");
          // Add it as a bracket to the op stack:
        }
        data.open_bracket("(");
        ++expr;
        break;
      case '[':
        if (data.lastTokenWasOp == false) {
          // If it is an operator:
          data.handle_op("[]");
        } else {
          // If it is the list constructor:
          // Add the list constructor to the rpn:
          data.handle_token(new CppFunction(&TokenList::default_constructor, "list"));

          // We make the program see it as a normal function call:
          data.handle_op("()");
        }
        // Add it as a bracket to the op stack:
        data.open_bracket("[");
        ++expr;
        break;
      case '{':
        // Add a map constructor call to the rpn:
        data.handle_token(new CppFunction(&TokenMap::default_constructor, "map"));

        // We make the program see it as a normal function call:
        data.handle_op("()");
        data.open_bracket("{");
        ++expr;
        break;
      case ')':
        data.close_bracket("(");
        ++expr;
        break;
      case ']':
        data.close_bracket("[");
        ++expr;
        break;
      case '}':
        data.close_bracket("{");
        ++expr;
        break;
      default:
        {
          // Then the token is an operator
          std::stringstream ss;
          ss << *expr;
          ++expr;
          while (*expr && ispunct(*expr) && !strchr("+-'\"()_", *expr)) {
            ss << *expr;
            ++expr;
          }
          std::string op = ss.str();

          rWordMap_t::iterator it;
          if ((it=rWordMap.find(op)) != rWordMap.end()) {
            // Parse reserved operators:
            try {
              it->second(expr, &expr, &data);
            } catch (...) {
              rpnBuilder::cleanRPN(&data.rpn);
              throw;
            }
          } else {
            data.handle_op(op);
          }
        }
      }
    }
    // Ignore spaces but stop on delimiter if not inside brackets.
    while (*expr && isspace(*expr)
           && (data.bracketLevel || !strchr(delim, *expr))) ++expr;
  }

  // Check for syntax errors (excess of operators i.e. 10 + + -1):
  if (data.lastTokenWasUnary) {
    rpnBuilder::cleanRPN(&data.rpn);
    throw syntax_error("Expected operand after unary operator `" + data.opStack.top() + "`");
  }

  while (!data.opStack.empty()) {
    data.rpn.push(new Token<std::string>(data.opStack.top(), OP));
    data.opStack.pop();
  }

  // In case one of the custom parsers left an empty expression:
  if (data.rpn.size() == 0) data.rpn.push(new TokenNone());
  if (rest) *rest = expr;
  return data.rpn;
}

packToken calculator::calculate(const char* expr, TokenMap vars,
                                const char* delim, const char** rest) {
  // Convert to RPN with Dijkstra's Shunting-yard algorithm.
  RAII_TokenQueue_t rpn = calculator::toRPN(expr, vars, delim, rest);

  TokenBase* ret = calculator::calculate(rpn, vars);

  return packToken(resolve_reference(ret));
}

void cleanStack(std::stack<TokenBase*> st) {
  while (st.size() > 0) {
    delete resolve_reference(st.top());
    st.pop();
  }
}

TokenBase* calculator::calculate(const TokenQueue_t& rpn, TokenMap scope,
                                 const opMap_t& opMap) {
  evaluationData data(rpn, scope, opMap);

  // Evaluate the expression in RPN form.
  std::stack<TokenBase*> evaluation;
  while (!data.rpn.empty()) {
    TokenBase* base = data.rpn.front()->clone();
    data.rpn.pop();

    // Operator:
    if (base->type == OP) {
      data.op = static_cast<Token<std::string>*>(base)->val;
      delete base;

      /* * * * * Resolve operands Values and References: * * * * */

      if (evaluation.size() < 2) {
        cleanStack(evaluation);
        throw std::domain_error("Invalid equation.");
      }
      TokenBase* b_right = evaluation.top(); evaluation.pop();
      TokenBase* b_left  = evaluation.top(); evaluation.pop();

      if (b_right->type == VAR) {
        std::string var_name = static_cast<Token<std::string>*>(b_right)->val;
        delete b_right;
        delete resolve_reference(b_left);
        cleanStack(evaluation);
        throw std::domain_error("Unable to find the variable '" + var_name + "'.");
      } else {
        b_right = resolve_reference(b_right, &data.scope);
      }

      packToken r_left;
      packToken m_left;
      if (b_left->type & REF) {
        RefToken* left = static_cast<RefToken*>(b_left);
        r_left = left->key;
        m_left = left->source;
        b_left = resolve_reference(left, &data.scope);
      } else if (b_left->type == VAR) {
        r_left = static_cast<Token<std::string>*>(b_left)->val;
      }

      /* * * * * Resolve Asign Operation * * * * */

      if (!data.op.compare("=")) {
        delete b_left;

        // If the left operand has a variable name:
        if (r_left->type == STR) {
          if (m_left->type == MAP) {
            TokenMap& map = m_left.asMap();
            std::string& key = r_left.asString();
            map[key] = packToken(b_right->clone());
          } else {
            TokenMap* map = data.scope.findMap(r_left.asString());
            if (!map || *map == TokenMap::default_global()) {
              // Assign on the local scope.
              // The user should not be able to implicitly overwrite
              // variables he did not declare, since it's error prone.
              data.scope[r_left.asString()] = packToken(b_right->clone());
            } else {
              (*map)[r_left.asString()] = packToken(b_right->clone());
            }
          }

          evaluation.push(b_right);
        // If the left operand has an index number:
        } else if (r_left->type & NUM) {
          if (m_left->type == LIST) {
            TokenList& list = m_left.asList();
            size_t index = r_left.asInt();
            list[index] = packToken(b_right->clone());
          } else {
            delete b_right;
            cleanStack(evaluation);
            throw std::domain_error("Left operand of assignment is not a list!");
          }

          evaluation.push(b_right);
        } else {
          packToken p_right(b_right->clone());
          delete b_right;

          cleanStack(evaluation);
          throw undefined_operation(data.op, r_left, p_right);
        }
      } else if (b_left->type == FUNC && data.op == "()") {
        Function* f_left = static_cast<Function*>(b_left);

        if (!data.op.compare("()")) {
          // Collect the parameter tuple:
          Tuple right;
          if (b_right->type == TUPLE) {
            right = *static_cast<Tuple*>(b_right);
          } else {
            right = Tuple(b_right);
          }
          delete b_right;

          packToken _this;
          if (m_left->type != NONE) {
            _this = m_left;
          } else {
            _this = data.scope;
          }

          // Execute the function:
          packToken ret;
          try {
            ret = Function::call(_this, f_left, &right, data.scope);
          } catch (...) {
            cleanStack(evaluation);
            delete f_left;
            throw;
          }

          delete f_left;

          evaluation.push(ret->clone());
        } else {
          packToken p_right(b_right->clone());
          packToken p_left(b_left->clone());
          delete b_right;

          cleanStack(evaluation);
          throw undefined_operation(data.op, p_left, p_right);
        }
      } else {
        data.opID = Operation::build_mask(b_left->type, b_right->type);
        packToken p_left(b_left);
        packToken p_right(b_right);
        TokenBase* result = 0;

        try {
          // Resolve the operation:
          result = exec_operation(p_left, p_right, &data, data.op);
          if (!result) {
            result = exec_operation(p_left, p_right, &data, ANY_OP);
          }
        } catch (...) {
          cleanStack(evaluation);
          throw;
        }

        if (result) {
          evaluation.push(result);
        } else {
          cleanStack(evaluation);
          throw undefined_operation(data.op, p_left, p_right);
        }
      }
    } else if (base->type == VAR) {  // Variable
      packToken* value = NULL;
      std::string key = static_cast<Token<std::string>*>(base)->val;

      value = data.scope.find(key);

      if (value) {
        TokenBase* copy = (*value)->clone();
        evaluation.push(new RefToken(key, copy));
        delete base;
      } else {
        evaluation.push(base);
      }
    } else {
      evaluation.push(base);
    }
  }

  return evaluation.top();
}

/* * * * * Non Static Functions * * * * */

calculator::~calculator() {
  rpnBuilder::cleanRPN(&this->RPN);
}

calculator::calculator(const calculator& calc) {
  TokenQueue_t _rpn = calc.RPN;

  // Deep copy the token list, so everything can be
  // safely deallocated:
  while (!_rpn.empty()) {
    TokenBase* base = _rpn.front();
    _rpn.pop();
    this->RPN.push(base->clone());
  }
}

// Work as a sub-parser:
// - Stops at delim or '\0'
// - Returns the rest of the string as char* rest
calculator::calculator(const char* expr, TokenMap vars, const char* delim,
                       const char** rest, const OppMap_t& opp,
                       const rWordMap_t& rwMap) {
  this->RPN = calculator::toRPN(expr, vars, delim, rest, opp, rwMap);
}

void calculator::compile(const char* expr, TokenMap vars, const char* delim,
                         const char** rest) {
  // Make sure it is empty:
  rpnBuilder::cleanRPN(&this->RPN);

  this->RPN = calculator::toRPN(expr, vars, delim, rest,
                                opPrecedence(), rWordMap());
}

packToken calculator::eval(TokenMap vars, bool keep_refs) const {
  TokenBase* value = calculate(this->RPN, vars, opMap());
  packToken p = packToken(value->clone());
  if (keep_refs) {
    return packToken(value);
  } else {
    return packToken(resolve_reference(value));
  }
}

calculator& calculator::operator=(const calculator& calc) {
  // Make sure the RPN is empty:
  rpnBuilder::cleanRPN(&this->RPN);

  // Deep copy the token list, so everything can be
  // safely deallocated:
  TokenQueue_t _rpn = calc.RPN;
  while (!_rpn.empty()) {
    TokenBase* base = _rpn.front();
    _rpn.pop();
    this->RPN.push(base->clone());
  }
  return *this;
}

/* * * * * For Debug Only * * * * */

std::string calculator::str() const {
  return str(this->RPN);
}

std::string calculator::str(TokenQueue_t rpn) {
  std::stringstream ss;

  ss << "calculator { RPN: [ ";
  while (rpn.size()) {
    ss << packToken(resolve_reference(rpn.front()->clone())).str();
    rpn.pop();

    ss << (rpn.size() ? ", ":"");
  }
  ss << " ] }";
  return ss.str();
}
