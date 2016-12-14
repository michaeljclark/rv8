#ifndef SHUNTING_YARD_H_
#define SHUNTING_YARD_H_

#include <map>
#include <stack>
#include <string>
#include <queue>
#include <list>
#include <vector>
#include <set>

/*
 * About tokType enum:
 *
 * The 3 left most bits (0x80, 0x40 and 0x20) of the Token Type
 * are reserved for denoting Numerals, Iterators and References.
 * If you want to define your own type please mind this bits.
 */
typedef uint8_t tokType_t;
typedef uint64_t opID_t;
enum tokType {
  // Base types:
  // Note: The mask system accepts at most 29 (32-3) different base types.
  NONE, OP, VAR, STR, FUNC,

  // Numerals:
  NUM = 0x20,   // Everything with the bit 0x20 set is a number.
  REAL = 0x21,  // == 0x20 => Real numbers.
  INT = 0x22,   // == 0x20 + 0x1 => Integers are numbers.

  // Complex types:
  IT = 0x40,      // Everything with the bit 0x20 set is an iterator.
  LIST = 0x41,    // == 0x20 + 0x01 => Lists are iterators.
  TUPLE = 0x42,   // == 0x20 + 0x02 => Tuples are iterators.
  STUPLE = 0x43,  // == 0x20 + 0x03 => ArgTuples are iterators.
  MAP = 0x44,     // == 0x20 + 0x04 => Maps are Iterators

  REF = 0x80,

  ANY_TYPE = 0xFF
};

#define ANY_OP ""

struct TokenBase {
  tokType_t type;
  virtual ~TokenBase() {}
  virtual TokenBase* clone() const = 0;
};

template<class T> class Token : public TokenBase {
 public:
  T val;
  Token(T t, tokType_t type) : val(t) { this->type = type; }
  virtual TokenBase* clone() const {
    return new Token(static_cast<const Token&>(*this));
  }
};

struct TokenNone : public TokenBase {
  TokenNone() { this->type = NONE; }
  virtual TokenBase* clone() const {
    return new TokenNone(static_cast<const TokenNone&>(*this));
  }
};

class packToken;
typedef std::queue<TokenBase*> TokenQueue_t;
class OppMap_t {
  // Set of operators that should be evaluated from right to left:
  std::set<std::string> RtoL;
  // Map of operators precedence:
  std::map<std::string, int> pr_map;

 public:
  OppMap_t() {
    // These operations are hard-coded inside the calculator,
    // thus their precedence should always be defined:
    pr_map["[]"] = -1; pr_map["()"] = -1;
    pr_map["["] = 0x7FFFFFFF; pr_map["("] = 0x7FFFFFFF; pr_map["{"] = 0x7FFFFFFF;
    RtoL.insert("=");
  }

  void add(const std::string& op, int precedence) {
    if (precedence < 0) {
      RtoL.insert(op);
      precedence = -precedence;
    }

    pr_map[op] = precedence;
  }

  int prec(const std::string& op) const { return pr_map.at(op); }
  bool assoc(const std::string& op) const { return RtoL.count(op); }
  bool exists(const std::string& op) const { return pr_map.count(op); }
};

struct TokenMap;
class TokenList;
class Tuple;
class STuple;
class Function;
#include "./packToken.h"

// Define the Tuple, TokenMap and TokenList classes:
#include "./objects.h"

// Define the `Function` class
// as well as some built-in functions:
#include "./functions.h"

// This struct was created to expose internal toRPN() variables
// to custom parsers, in special to the rWordParser_t functions.
struct rpnBuilder {
  TokenQueue_t rpn;
  std::stack<std::string> opStack;
  uint8_t lastTokenWasOp = true;
  bool lastTokenWasUnary = false;
  TokenMap scope;
  const OppMap_t& opp;

  // Used to make sure the expression won't
  // end inside a bracket evaluation just because
  // found a delimiter like '\n' or ')'
  uint32_t bracketLevel = 0;

  rpnBuilder(TokenMap scope, const OppMap_t& opp) : scope(scope), opp(opp) {}

 public:
  static void cleanRPN(TokenQueue_t* rpn);

 public:
  void handle_op(const std::string& op);
  void handle_token(TokenBase* token);
  void open_bracket(const std::string& bracket);
  void close_bracket(const std::string& bracket);

 private:
  void handle_unary(const std::string& op);
};

struct opMap_t;
struct evaluationData {
  TokenQueue_t rpn;
  TokenMap scope;
  const opMap_t& opMap;

  std::string op;
  opID_t opID;

  evaluationData(TokenQueue_t rpn, TokenMap scope, const opMap_t& opMap)
                : rpn(rpn), scope(scope), opMap(opMap) {}
};

// The reservedWordParser_t is the function type called when
// a reserved word is found at parsing time.
typedef void rWordParser_t(const char* expr, const char** rest,
                           rpnBuilder* data);
typedef std::map<std::string, rWordParser_t*> rWordMap_t;

struct RefToken : public TokenBase {
  packToken key;
  packToken value;
  packToken source;
  RefToken(packToken k, TokenBase* v, packToken m = packToken::None) :
    key(k), value(v), source(m) { this->type = v->type | REF; }
  RefToken(packToken k, packToken v, packToken m = packToken::None) :
    key(k), value(v), source(m) { this->type = v->type | REF; }

  virtual TokenBase* clone() const {
    return new RefToken(*this);
  }
};

struct opSignature_t {
  tokType_t left; std::string op; tokType_t right;
  opSignature_t(const tokType_t L, const std::string op, const tokType_t R)
               : left(L), op(op), right(R) {}
};

class Operation {
 public:
  typedef packToken (*opFunc_t)(const packToken& left, const packToken& right,
                                evaluationData* data);

 public:
  // Use this exception to reject an operation.
  // Without stoping the operation matching process.
  struct Reject : public std::exception {};

 public:
  static inline const uint32_t mask(tokType_t type);
  static const opID_t build_mask(tokType_t left, tokType_t right);

 private:
  opID_t _mask;
  opFunc_t _exec;

 public:
  Operation(opSignature_t sig, opFunc_t func)
           : _mask(build_mask(sig.left, sig.right)), _exec(func) {}

 public:
  const opID_t getMask() const { return _mask; }
  packToken exec(const packToken& left, const packToken& right,
                 evaluationData* data) const {
    return _exec(left, right, data);
  }
};

typedef std::map<tokType_t, TokenMap> typeMap_t;
typedef std::vector<Operation> opList_t;
struct opMap_t : public std::map<std::string, opList_t> {
  void add(const opSignature_t sig, Operation::opFunc_t func) {
    (*this)[sig.op].push_back(Operation(sig, func));
  }
};

class calculator {
 public:
  static rWordMap_t& default_rWordMap();
  static OppMap_t& default_opPrecedence();
  static opMap_t& default_opMap();

 public:
  static typeMap_t& type_attribute_map();

 public:
  static packToken calculate(const char* expr, TokenMap vars = &TokenMap::empty,
                             const char* delim = 0, const char** rest = 0);

 public:
  static TokenBase* calculate(const TokenQueue_t& RPN, TokenMap scope,
                              const opMap_t& opMap = default_opMap());
  static TokenQueue_t toRPN(const char* expr, TokenMap vars,
                            const char* delim = 0, const char** rest = 0,
                            OppMap_t opPrecedence = default_opPrecedence(),
                            rWordMap_t rWordMap = default_rWordMap());

 public:
  // Used to dealloc a TokenQueue_t safely.
  struct RAII_TokenQueue_t;

 protected:
  virtual const opMap_t opMap() const { return default_opMap(); }
  virtual const OppMap_t opPrecedence() const { return default_opPrecedence(); }
  virtual const rWordMap_t rWordMap() const { return default_rWordMap(); }

 private:
  TokenQueue_t RPN;

 public:
  virtual ~calculator();
  calculator() { this->RPN.push(new TokenNone()); }
  calculator(const calculator& calc);
  calculator(const char* expr, TokenMap vars = &TokenMap::empty,
             const char* delim = 0, const char** rest = 0,
             const OppMap_t& opPrecedence = default_opPrecedence(),
             const rWordMap_t& rwMap = default_rWordMap());
  void compile(const char* expr, TokenMap vars = &TokenMap::empty,
               const char* delim = 0, const char** rest = 0);
  packToken eval(TokenMap vars = &TokenMap::empty, bool keep_refs = false) const;

  // Serialization:
  std::string str() const;
  static std::string str(TokenQueue_t rpn);

  // Operators:
  calculator& operator=(const calculator& calc);
};

#endif  // SHUNTING_YARD_H_
