#include <sstream>
#include <string>
#include <iostream>

#include "./shunting-yard.h"
#include "./packToken.h"
#include "./shunting-yard-exceptions.h"

const packToken packToken::None = packToken(TokenNone());

packToken::packToken(const TokenMap& map) : base(new TokenMap(map)) {}
packToken::packToken(const TokenList& list) : base(new TokenList(list)) {}

packToken& packToken::operator=(const packToken& t) {
  delete base;
  base = t.base->clone();
  return *this;
}

bool packToken::operator==(const packToken& token) const {
  if (NUM & token.base->type & base->type) {
    return token.asDouble() == asDouble();
  }

  if (token.base->type != base->type) {
    return false;
  } else {
    // Compare strings to simplify code
    return token.str().compare(str()) == 0;
  }
}

bool packToken::operator!=(const packToken& token) const {
  return !(*this == token);
}

TokenBase* packToken::operator->() const {
  return base;
}

std::ostream& operator<<(std::ostream &os, const packToken& t) {
  return os << t.str();
}

packToken& packToken::operator[](const std::string& key) {
  if (base->type != MAP) {
    throw bad_cast(
      "The Token is not a map!");
  }
  return (*static_cast<TokenMap*>(base))[key];
}
const packToken& packToken::operator[](const std::string& key) const {
  if (base->type != MAP) {
    throw bad_cast(
      "The Token is not a map!");
  }
  return (*static_cast<TokenMap*>(base))[key];
}
packToken& packToken::operator[](const char* key) {
  if (base->type != MAP) {
    throw bad_cast(
      "The Token is not a map!");
  }
  return (*static_cast<TokenMap*>(base))[key];
}
const packToken& packToken::operator[](const char* key) const {
  if (base->type != MAP) {
    throw bad_cast(
      "The Token is not a map!");
  }
  return (*static_cast<TokenMap*>(base))[key];
}

bool packToken::asBool() const {
  switch (base->type) {
    case REAL:
      return static_cast<Token<double>*>(base)->val != 0;
    case INT:
      return static_cast<Token<int64_t>*>(base)->val != 0;
    case STR:
      return static_cast<Token<std::string>*>(base)->val != std::string();
    case MAP:
    case FUNC:
      return true;
    case NONE:
      return false;
    case TUPLE:
    case STUPLE:
      return static_cast<Tuple*>(base)->list().size() != 0;
    default:
      throw bad_cast("Token type can not be cast to boolean!");
  }
}

double packToken::asDouble() const {
  if (!(base->type & NUM)) {
    throw bad_cast(
      "The Token is not a number!");
  }
  if (base->type == REAL) {
    return static_cast<Token<double>*>(base)->val;
  } else {
    return static_cast<Token<int64_t>*>(base)->val;
  }
}

int64_t packToken::asInt() const {
  if (!(base->type & NUM)) {
    throw bad_cast(
      "The Token is not a number!");
  }
  if (base->type == REAL) {
    return static_cast<Token<double>*>(base)->val;
  } else {
    return static_cast<Token<int64_t>*>(base)->val;
  }
}

std::string& packToken::asString() const {
  if (base->type != STR && base->type != VAR && base->type != OP) {
    throw bad_cast(
      "The Token is not a string!");
  }
  return static_cast<Token<std::string>*>(base)->val;
}

TokenMap& packToken::asMap() const {
  if (base->type != MAP) {
    throw bad_cast(
      "The Token is not a map!");
  }
  return *static_cast<TokenMap*>(base);
}

TokenList& packToken::asList() const {
  if (base->type != LIST) {
    throw bad_cast(
      "The Token is not a list!");
  }
  return *static_cast<TokenList*>(base);
}

Tuple& packToken::asTuple() const {
  if (base->type != TUPLE) {
    throw bad_cast(
      "The Token is not a tuple!");
  }
  return *static_cast<Tuple*>(base);
}

STuple& packToken::asSTuple() const {
  if (base->type != STUPLE) {
    throw bad_cast(
      "The Token is not an special tuple!");
  }
  return *static_cast<STuple*>(base);
}

Function* packToken::asFunc() const {
  if (base->type != FUNC) {
    throw bad_cast(
      "The Token is not a function!");
  }
  return static_cast<Function*>(base);
}

std::string packToken::str() const {
  return packToken::str(base);
}

std::string packToken::str(const TokenBase* base) {
  std::stringstream ss;
  TokenMap_t* tmap;
  TokenMap_t::iterator m_it;

  TokenList_t* tlist;
  TokenList_t::iterator l_it;
  const Function* func;
  bool first;
  std::string name;

  if (!base) return "undefined";

  if (base->type & REF) {
    base = static_cast<const RefToken*>(base)->value;
    name = static_cast<const RefToken*>(base)->key.str();
  }

  switch (base->type) {
    case NONE:
      return "None";
    case OP:
      return static_cast<const Token<std::string>*>(base)->val;
    case VAR:
      return static_cast<const Token<std::string>*>(base)->val;
    case REAL:
      ss << static_cast<const Token<double>*>(base)->val;
      return ss.str();
    case INT:
      ss << static_cast<const Token<int64_t>*>(base)->val;
      return ss.str();
    case STR:
      return "\"" + static_cast<const Token<std::string>*>(base)->val + "\"";
    case FUNC:
      func = static_cast<const Function*>(base);
      if (func->name().size()) return "[Function: " + func->name() + "]";
      if (name.size()) return "[Function: " + name + "]";
      return "[Function]";
    case TUPLE:
    case STUPLE:
      ss << "(";
      first = true;
      for (const TokenBase* token : static_cast<const Tuple*>(base)->list()) {
        if (!first) {
          ss << ", ";
        } else {
          first = false;
        }
        ss << str(token);
      }
      if (first) {
        // Its an empty tuple:
        // Add a `,` to make it different than ():
        ss << ",)";
      } else {
        ss << ")";
      }
      return ss.str();
    case MAP:
      tmap = &(static_cast<const TokenMap*>(base)->map());
      if (tmap->size() == 0) return "{}";
      ss << "{";
      for (m_it = tmap->begin(); m_it != tmap->end(); ++m_it) {
        ss << (m_it == tmap->begin() ? "" : ",");
        ss << " \"" << m_it->first << "\": " << m_it->second.str();
      }
      ss << " }";
      return ss.str();
    case LIST:
      tlist = &(static_cast<const TokenList*>(base)->list());
      if (tlist->size() == 0) return "[]";
      ss << "[";
      for (l_it = tlist->begin(); l_it != tlist->end(); ++l_it) {
        ss << (l_it == tlist->begin() ? "" : ",");
        ss << " " << l_it->str();
      }
      ss << " ]";
      return ss.str();
    default:
      if (base->type & IT) {
        return "[Iterator]";
      }
      return "unknown_type";
  }
}
