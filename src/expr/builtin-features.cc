#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>
#include <stdexcept>
#include <cerrno>
#include <sstream>
#include <iostream>
#include <cctype>  // For tolower() and toupper()

#include "./shunting-yard.h"
#include "./shunting-yard-exceptions.h"

namespace builtin_operations {

packToken Comma(const packToken& left, const packToken& right, evaluationData* data) {
  if (left->type == TUPLE) {
    left.asTuple().list().push_back(right);
    return left;
  } else {
    return Tuple(left, right);
  }
}

packToken Colon(const packToken& left, const packToken& right, evaluationData* data) {
  if (left->type == STUPLE) {
    left.asSTuple().list().push_back(right);
    return left;
  } else {
    return STuple(left, right);
  }
}

packToken Equal(const packToken& left, const packToken& right, evaluationData* data) {
  if (left->type == VAR || right->type == VAR) {
    throw undefined_operation(data->op, left, right);
  }

  return left == right;
}

packToken Different(const packToken& left, const packToken& right, evaluationData* data) {
  if (left->type == VAR || right->type == VAR) {
    throw undefined_operation(data->op, left, right);
  }

  return left != right;
}

packToken MapIndex(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  TokenMap& left = p_left.asMap();
  std::string& right = p_right.asString();
  const std::string& op = data->op;

  if (op == "[]" || op == ".") {
    packToken* p_value = left.find(right);

    if (p_value) {
      return RefToken(right, *p_value, left);
    } else {
      return RefToken(right, packToken::None, left);
    }
  } else {
    throw undefined_operation(op, left, right);
  }
}

// Resolve build-in operations for non-map types, e.g.: 'str'.len()
packToken TypeSpecificFunction(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  if (p_left->type == MAP) throw Operation::Reject();

  TokenMap& attr_map = calculator::type_attribute_map()[p_left->type];
  std::string& key = p_right.asString();

  packToken* attr = attr_map.find(key);
  if (attr) {
    // Note: If attr is a function, it will receive have
    // scope["this"] == source, so it can make changes on this object.
    // Or just read some information for example: its length.
    return RefToken(key, (*attr), p_left);
  } else {
    throw undefined_operation(data->op, p_left, p_right);
  }
}

packToken NumeralOperation(const packToken& left, const packToken& right, evaluationData* data) {
  double left_d, right_d;
  int64_t left_i, right_i;

  // Extract integer and real values of the operators:
  left_d = left.asDouble();
  left_i = left.asInt();

  right_d = right.asDouble();
  right_i = right.asInt();

  const std::string& op = data->op;

  if (op == "+") {
    return left_d + right_d;
  } else if (op == "*") {
    return left_d * right_d;
  } else if (op == "-") {
    return left_d - right_d;
  } else if (op == "/") {
    return left_d / right_d;
  } else if (op == "<<") {
    return left_i << right_i;
  } else if (op == "**") {
    return pow(left_d, right_d);
  } else if (op == ">>") {
    return left_i >> right_i;
  } else if (op == "%") {
    return left_i % right_i;
  } else if (op == "<") {
    return left_d < right_d;
  } else if (op == ">") {
    return left_d > right_d;
  } else if (op == "<=") {
    return left_d <= right_d;
  } else if (op == ">=") {
    return left_d >= right_d;
  } else if (op == "&&") {
    return left_i && right_i;
  } else if (op == "||") {
    return left_i || right_i;
  } else {
    throw undefined_operation(op, left, right);
  }
}

packToken FormatOperation(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  std::string& s_left = p_left.asString();
  const char* left = s_left.c_str();

  Tuple right;

  if (p_right->type == TUPLE) {
    right = p_right.asTuple();
  } else {
    right = Tuple(p_right);
  }

  std::string result;
  for (const packToken& token : right.list()) {
    // Find the next occurrence of "%s"
    while (*left && (*left != '%' || left[1] != 's')) {
      if (*left == '\\' && left[1] == '%') ++left;
      result.push_back(*left);
      ++left;
    }

    if (*left == '\0') {
      throw type_error("Not all arguments converted during string formatting");
    } else {
      left += 2;
    }

    // Replace it by the token string representation:
    if (token->type == STR) {
      // Avoid using packToken::str for strings
      // or it will enclose it quotes `"str"`
      result += token.asString();
    } else {
      result += token.str();
    }
  }

  // Find the next occurrence of "%s" if exists:
  while (*left && (*left != '%' || left[1] != 's')) {
    if (*left == '\\' && left[1] == '%') ++left;
    result.push_back(*left);
    ++left;
  }

  if (*left != '\0') {
    throw type_error("Not enough arguments for format string");
  } else {
    return result;
  }
}

packToken StringOnStringOperation(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  const std::string& left = p_left.asString();
  const std::string& right = p_right.asString();
  const std::string& op = data->op;

  if (op == "+") {
    return left + right;
  } else if (op == "==") {
    return (left == right);
  } else if (op == "!=") {
    return (left != right);
  } else {
    throw undefined_operation(op, p_left, p_right);
  }
}

packToken StringOnNumberOperation(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  const std::string& left = p_left.asString();
  const std::string& op = data->op;

  std::stringstream ss;
  if (op == "+") {
    ss << left << p_right.asDouble();
    return ss.str();
  } else if (op == "[]") {
    int64_t index = p_right.asInt();

    if (index < 0) {
      // Reverse index, i.e. list[-1] = list[list.size()-1]
      index += left.size();
    }
    if (index < 0 || static_cast<size_t>(index) >= left.size()) {
      throw std::domain_error("String index out of range!");
    }

    ss << left[index];
    return ss.str();
  } else {
    throw undefined_operation(op, p_left, p_right);
  }
}

packToken NumberOnStringOperation(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  double left = p_left.asDouble();
  const std::string& right = p_right.asString();

  std::stringstream ss;
  if (data->op == "+") {
    ss << left << right;
    return ss.str();
  } else {
    throw undefined_operation(data->op, p_left, p_right);
  }
}

packToken ListOnNumberOperation(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  TokenList left = p_left.asList();

  if (data->op == "[]") {
    int64_t index = p_right.asInt();

    if (index < 0) {
      // Reverse index, i.e. list[-1] = list[list.size()-1]
      index += left.list().size();
    }

    if (index < 0 || static_cast<size_t>(index) >= left.list().size()) {
      throw std::domain_error("List index out of range!");
    }

    packToken& value = left.list()[index];

    return RefToken(index, value, p_left);
  } else {
    throw undefined_operation(data->op, p_left, p_right);
  }
}

packToken ListOnListOperation(const packToken& p_left, const packToken& p_right, evaluationData* data) {
  TokenList& left = p_left.asList();
  TokenList& right = p_right.asList();

  if (data->op == "+") {
    // Deep copy the first list:
    TokenList result;
    result.list() = left.list();

    // Insert items from the right list into the left:
    for (packToken& p : right.list()) {
      result.list().push_back(p);
    }

    return result;
  } else {
    throw undefined_operation(data->op, left, right);
  }
}

struct Startup {
  Startup() {
    // Create the operator precedence map based on C++ default
    // precedence order as described on cppreference website:
    // http://en.cppreference.com/w/cpp/language/operator_precedence
    OppMap_t& opp = calculator::default_opPrecedence();
    opp.add("[]", 2); opp.add("()", 2); opp.add(".", 2);
    opp.add("**", 3);
    opp.add("*",  5); opp.add("/", 5); opp.add("%", 5);
    opp.add("+",  6); opp.add("-", 6);
    opp.add("<<", 7); opp.add(">>", 7);
    opp.add("<",  8); opp.add("<=", 8); opp.add(">=", 8); opp.add(">", 8);
    opp.add("==", 9); opp.add("!=", 9);
    opp.add("&&", 13);
    opp.add("||", 14);
    opp.add("=", 15); opp.add(":", 15);
    opp.add(",", 16);

    // Link operations to respective operators:
    opMap_t& opMap = calculator::default_opMap();
    opMap.add({ANY_TYPE, ",", ANY_TYPE}, &Comma);
    opMap.add({ANY_TYPE, ":", ANY_TYPE}, &Colon);
    opMap.add({ANY_TYPE, "==", ANY_TYPE}, &Equal);
    opMap.add({ANY_TYPE, "!=", ANY_TYPE}, &Different);
    opMap.add({MAP, "[]", STR}, &MapIndex);
    opMap.add({ANY_TYPE, ".", STR}, &TypeSpecificFunction);
    opMap.add({MAP, ".", STR}, &MapIndex);
    opMap.add({STR, "%", ANY_TYPE}, &FormatOperation);

    // Note: The order is important:
    opMap.add({NUM, ANY_OP, NUM}, &NumeralOperation);
    opMap.add({STR, ANY_OP, STR}, &StringOnStringOperation);
    opMap.add({STR, ANY_OP, NUM}, &StringOnNumberOperation);
    opMap.add({NUM, ANY_OP, STR}, &NumberOnStringOperation);
    opMap.add({LIST, ANY_OP, NUM}, &ListOnNumberOperation);
    opMap.add({LIST, ANY_OP, LIST}, &ListOnListOperation);
  }
} StartUp;

}  // namespace builtin_operations

namespace builtin_reservedWords {

// Literal Tokens: True, False and None:
packToken trueToken = packToken(1);
packToken falseToken = packToken(0);
packToken noneToken = packToken(0);

void True(const char* expr, const char** rest, rpnBuilder* data) {
  data->handle_token(trueToken->clone());
}

void False(const char* expr, const char** rest, rpnBuilder* data) {
  data->handle_token(falseToken->clone());
}

void None(const char* expr, const char** rest, rpnBuilder* data) {
  data->handle_token(noneToken->clone());
}

void LineComment(const char* expr, const char** rest, rpnBuilder* data) {
  while (*expr && *expr != '\n') ++expr;
  *rest = expr;
}

void SlashStarComment(const char* expr, const char** rest, rpnBuilder* data) {
  while (*expr && !(expr[0] == '*' && expr[1] == '/')) ++expr;
  if (*expr == '\0') {
    throw syntax_error("Unexpected end of file after '/*' comment!");
  }
  // Drop the characters `*/`:
  expr += 2;
  *rest = expr;
}

struct Startup {
  Startup() {
    rWordMap_t& rwMap = calculator::default_rWordMap();
    rwMap["True"] = &True;
    rwMap["False"] = &False;
    rwMap["None"] = &None;
    rwMap["#"] = &LineComment;
    rwMap["//"] = &LineComment;
    rwMap["/*"] = &SlashStarComment;
  }
} Startup;

}  // namespace builtin_reservedWords

namespace builtin_functions {

/* * * * * Built-in Functions: * * * * */

packToken default_print(TokenMap scope) {
  // Get the argument list:
  TokenList list = scope["args"].asList();

  bool first = true;
  for (packToken item : list.list()) {
    if (first) {
      first = false;
    } else {
      std::cout << " ";
    }

    if (item->type == STR) {
      std::cout << item.asString();
    } else {
      std::cout << item.str();
    }
  }

  std::cout << std::endl;

  return packToken::None;
}

packToken default_sum(TokenMap scope) {
  // Get the arguments:
  TokenList list = scope["args"].asList();

  if (list.list().size() == 1 && list.list().front()->type == LIST) {
    list = list.list().front().asList();
  }

  double sum = 0;
  for (packToken num : list.list()) {
    sum += num.asDouble();
  }

  return sum;
}

const args_t value_arg = {"value"};
packToken default_eval(TokenMap scope) {
  std::string code = scope["value"].asString();
  // Evaluate it as a calculator expression:
  return calculator::calculate(code.c_str(), scope);
}

packToken default_float(TokenMap scope) {
  packToken tok = scope["value"];
  if (tok->type & NUM) return tok.asDouble();

  // Convert it to double:
  char* rest;
  const std::string& str = tok.asString();
  errno = 0;
  double ret = strtod(str.c_str(), &rest);

  if (str == rest) {
    throw std::runtime_error("Could not convert \"" + str + "\" to float!");
  } else if (errno) {
    std::range_error("Value too big or too small to fit a Double!");
  }
  return ret;
}

packToken default_int(TokenMap scope) {
  packToken tok = scope["value"];
  if (tok->type & NUM) return tok.asInt();

  // Convert it to double:
  char* rest;
  const std::string& str = tok.asString();
  errno = 0;
  int64_t ret = strtol(str.c_str(), &rest, 10);

  if (str == rest) {
    throw std::runtime_error("Could not convert \"" + str + "\" to integer!");
  } else if (errno) {
    std::range_error("Value too big or too small to fit an Integer!");
  }
  return ret;
}

packToken default_str(TokenMap scope) {
  // Return its string representation:
  packToken tok = scope["value"];
  if (tok->type == STR) return tok;
  return tok.str();
}

packToken default_type(TokenMap scope) {
  packToken tok = scope["value"];
  switch (tok->type) {
  case NONE: return "none";
  case VAR: return "variable";
  case REAL: return "float";
  case INT: return "integer";
  case STR: return "string";
  case FUNC: return "function";
  case IT: return "iterable";
  case TUPLE: return "tuple";
  case STUPLE: return "argument tuple";
  case LIST: return "list";
  case MAP: return "map";
  default: return "unknown_type";
  }
}

const args_t num_arg = {"number"};
packToken default_sqrt(TokenMap scope) {
  // Get a single argument:
  double number = scope["number"].asDouble();

  return sqrt(number);
}
packToken default_sin(TokenMap scope) {
  // Get a single argument:
  double number = scope["number"].asDouble();

  return sin(number);
}
packToken default_cos(TokenMap scope) {
  // Get a single argument:
  double number = scope["number"].asDouble();

  return cos(number);
}
packToken default_tan(TokenMap scope) {
  // Get a single argument:
  double number = scope["number"].asDouble();

  return tan(number);
}
packToken default_abs(TokenMap scope) {
  // Get a single argument:
  double number = scope["number"].asDouble();

  return std::abs(number);
}

const args_t pow_args = {"number", "exp"};
packToken default_pow(TokenMap scope) {
  // Get two arguments:
  double number = scope["number"].asDouble();
  double exp = scope["exp"].asDouble();

  return pow(number, exp);
}

/* * * * * Type-specific default functions * * * * */

packToken string_len(TokenMap scope) {
  std::string str = scope["this"].asString();
  return static_cast<int64_t>(str.size());
}

packToken string_lower(TokenMap scope) {
  std::string str = scope["this"].asString();
  std::string out;
  for (char c : str) {
    out.push_back(tolower(c));
  }
  return out;
}

packToken string_upper(TokenMap scope) {
  std::string str = scope["this"].asString();
  std::string out;
  for (char c : str) {
    out.push_back(toupper(c));
  }
  return out;
}

/* * * * * default constructor functions * * * * */

packToken default_list(TokenMap scope) {
  // Get the arguments:
  TokenList list = scope["args"].asList();

  // If the only argument is iterable:
  if (list.list().size() == 1 && list.list()[0]->type & IT) {
    TokenList new_list;
    Iterator* it = static_cast<Iterable*>(list.list()[0].token())->getIterator();

    packToken* next = it->next();
    while (next) {
      new_list.list().push_back(*next);
      next = it->next();
    }

    delete it;
    return new_list;
  } else {
    return list;
  }
}

packToken default_map(TokenMap scope) {
  return scope["kwargs"];
}

/* * * * * Object inheritance tools: * * * * */

packToken default_extend(TokenMap scope) {
  packToken tok = scope["value"];

  if (tok->type == MAP) {
    return tok.asMap().getChild();
  } else {
    throw std::runtime_error(tok.str() + " is not extensible!");
  }
}

packToken default_instanceof(TokenMap scope) {
  TokenMap _super = scope["value"].asMap();
  TokenMap* _this = scope["this"].asMap().parent();

  TokenMap* parent = _this;
  while (parent) {
    if ((*parent) == _super) {
      return true;
    }

    parent = parent->parent();
  }

  return false;
}

}  // namespace builtin_functions

/* * * * * class CppFunction * * * * */

CppFunction::CppFunction(packToken (*func)(TokenMap), const args_t args,
                         std::string name)
                         : func(func), _args(args) {
  this->_name = name;
}

CppFunction::CppFunction(packToken (*func)(TokenMap), unsigned int nargs,
                         const char** args, std::string name)
                         : func(func) {
  this->_name = name;
  // Add all strings to args list:
  for (uint32_t i = 0; i < nargs; ++i) {
    this->_args.push_back(args[i]);
  }
}

// Build a function with no named args:
CppFunction::CppFunction(packToken (*func)(TokenMap), std::string name)
                         : func(func) {
  this->_name = name;
}

/* * * * * CppFunction Initializer Constructor: * * * * */

namespace builtin_functions {

struct Startup {
  Startup() {
    TokenMap& global = TokenMap::default_global();

    global["print"] = CppFunction(&default_print, "print");
    global["sum"] = CppFunction(&default_sum, "sum");
    global["sqrt"] = CppFunction(&default_sqrt, num_arg, "sqrt");
    global["sin"] = CppFunction(&default_sin, num_arg, "sin");
    global["cos"] = CppFunction(&default_cos, num_arg, "cos");
    global["tan"] = CppFunction(&default_tan, num_arg, "tan");
    global["abs"] = CppFunction(&default_abs, num_arg, "abs");
    global["pow"] = CppFunction(&default_pow, pow_args, "pow");
    global["float"] = CppFunction(&default_float, value_arg, "float");
    global["str"] = CppFunction(&default_str, value_arg, "str");
    global["eval"] = CppFunction(&default_eval, value_arg, "eval");
    global["type"] = CppFunction(&default_type, value_arg, "type");
    global["extend"] = CppFunction(&default_extend, value_arg, "extend");

    // Default constructors:
    global["list"] = CppFunction(&default_list, "list");
    global["map"] = CppFunction(&default_map, "map");

    TokenMap& base_map = TokenMap::base_map();
    base_map["instanceof"] = CppFunction(&default_instanceof,
                                         value_arg, "instanceof");

    typeMap_t& type_map = calculator::type_attribute_map();
    type_map[STR]["len"] = CppFunction(&string_len, "len");
    type_map[STR]["lower"] = CppFunction(&string_lower, "lower");
    type_map[STR]["upper"] = CppFunction(&string_upper, "upper");
  }
} base_functions_startup;

}  // namespace builtin_functions

namespace builtin_typeSpecificFunctions {

/* * * * * TokenMap built-in functions * * * * */

const args_t map_pop_args = {"key", "default"};
packToken map_pop(TokenMap scope) {
  TokenMap map = scope["this"].asMap();
  std::string key = scope["key"].asString();

  // Check if the item is available and remove it:
  if (map.map().count(key)) {
    packToken value = map[key];
    map.erase(key);
    return value;
  }

  // If not available return the default value or None
  packToken* def = scope.find("default");
  if (def) {
    return *def;
  } else {
    return packToken::None;
  }
}

packToken map_len(TokenMap scope) {
  TokenMap map = scope.find("this")->asMap();
  return map.map().size();
}

/* * * * * TokenList built-in functions * * * * */

const args_t push_args = {"item"};
packToken list_push(TokenMap scope) {
  packToken* list = scope.find("this");
  packToken* token = scope.find("item");

  // If "this" is not a list it will throw here:
  list->asList().list().push_back(*token);

  return *list;
}

const args_t list_pop_args = {"pos"};
packToken list_pop(TokenMap scope) {
  TokenList list = scope.find("this")->asList();
  packToken* token = scope.find("pos");

  int64_t pos;

  if ((*token)->type & NUM) {
    pos = token->asInt();

    // So that pop(-1) is the same as pop(last_idx):
    if (pos < 0) pos = list.list().size()-pos;
  } else {
    pos = list.list().size()-1;
  }

  packToken result = list.list()[pos];

  // Erase the item from the list:
  // Note that this operation is optimal if pos == list.size()-1
  list.list().erase(list.list().begin() + pos);

  return result;
}

packToken list_len(TokenMap scope) {
  TokenList list = scope.find("this")->asList();
  return list.list().size();
}

/* * * * * Initialize TokenList functions * * * * */

struct Startup {
  Startup() {
    TokenMap& base_list = calculator::type_attribute_map()[LIST];
    base_list["push"] = CppFunction(list_push, push_args, "push");
    base_list["pop"] = CppFunction(list_pop, list_pop_args, "pop");
    base_list["len"] = CppFunction(list_len, "len");

    TokenMap& base_map = TokenMap::base_map();
    base_map["pop"] = CppFunction(map_pop, map_pop_args, "pop");
    base_map["len"] = CppFunction(map_len, "len");
  }
} list_startup;

}  // namespace builtin_typeSpecificFunctions
