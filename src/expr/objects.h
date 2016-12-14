
#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <map>
#include <list>
#include <vector>
#include <string>

// The pack template class manages
// reference counting.
#include "./pack.h"

struct Iterator;

struct Iterable : public TokenBase {
  virtual ~Iterable() {}
  virtual Iterator* getIterator() = 0;
};

// Iterator super class.
struct Iterator : public Iterable {
  Iterator() { this->type = IT; }
  virtual ~Iterator() {}
  // Return the next position of the iterator.
  // When it reaches the end it should return NULL
  // and reset the iterator automatically.
  virtual packToken* next() = 0;
  virtual void reset() = 0;

  Iterator* getIterator();
};

struct TokenMap;
typedef std::map<std::string, packToken> TokenMap_t;

struct MapData_t {
  TokenMap_t map;
  TokenMap* parent;
  MapData_t(TokenMap* p);
  MapData_t(const MapData_t& other);
  ~MapData_t();

  MapData_t& operator=(const MapData_t& other);
};

struct TokenMap : public pack<MapData_t>, public Iterable {
  // Static factories:
  static TokenMap empty;
  static TokenMap& base_map();
  static TokenMap& default_global();
  static packToken default_constructor(TokenMap scope);

 public:
  // Attribute getters for the
  // pack<MapData_t> super class:
  TokenMap_t& map() const { return ref->obj->map; }
  TokenMap* parent() const { return ref->obj->parent; }

 public:
  // Implement the Iterable Interface:
  struct MapIterator : public Iterator {
    const TokenMap_t& map;
    TokenMap_t::const_iterator it = map.begin();
    packToken last;

    MapIterator(const TokenMap_t& map) : map(map) {}

    packToken* next();
    void reset();

    TokenBase* clone() const {
      return new MapIterator(*this);
    }
  };

  Iterator* getIterator() {
    return new MapIterator(map());
  }

 public:
  TokenMap(TokenMap* parent = &TokenMap::base_map()) : pack(parent) {
    // For the TokenBase super class
    this->type = MAP;
  }
  TokenMap(const TokenMap& other) : pack(other) { this->type = MAP; }
  virtual ~TokenMap() {}

 public:
  // Implement the TokenBase abstract class
  TokenBase* clone() const {
    return new TokenMap(*this);
  }

 public:
  packToken* find(std::string key);
  TokenMap* findMap(std::string key);
  void assign(std::string key, TokenBase* value);
  void insert(std::string key, TokenBase* value);

  TokenMap getChild();

  packToken& operator[](const std::string& str);

  void erase(std::string key);
};

// Build a TokenMap which is a child of default_global()
struct GlobalScope : public TokenMap {
  GlobalScope() : TokenMap(&TokenMap::default_global()) {}
};

typedef std::vector<packToken> TokenList_t;

class TokenList : public pack<TokenList_t>, public Iterable {
 public:
  static packToken default_constructor(TokenMap scope);

 public:
  // Attribute getter for the
  // pack<TokenList_t> super class:
  TokenList_t& list() const { return *(ref->obj); }

 public:
  struct ListIterator : public Iterator {
    TokenList_t* list;
    uint64_t i = 0;

    ListIterator(TokenList_t* list) : list(list) {}

    packToken* next();
    void reset();

    TokenBase* clone() const {
      return new ListIterator(*this);
    }
  };

  Iterator* getIterator() {
    return new ListIterator(&list());
  }

 public:
  TokenList() { this->type = LIST; }
  virtual ~TokenList() {}

  packToken& operator[](const uint64_t idx) const {
    if (list().size() <= idx) {
      throw std::out_of_range("List index out of range!");
    }
    return list()[idx];
  }

  void push(packToken val) const { list().push_back(val); }
  packToken pop() const {
    packToken back = list().back();
    list().pop_back();
    return back;
  }

 public:
  // Implement the TokenBase abstract class
  TokenBase* clone() const {
    return new TokenList(*this);
  }
};

class Tuple : public TokenList {
 public:
  Tuple() { this->type = TUPLE; }
  Tuple(const TokenBase* first) {
    this->type = TUPLE;
    list().push_back(packToken(first->clone()));
  }

  Tuple(const TokenBase* first, const TokenBase* second) {
    this->type = TUPLE;
    list().push_back(packToken(first->clone()));
    list().push_back(packToken(second->clone()));
  }

 public:
  // Implement the TokenBase abstract class
  TokenBase* clone() const {
    return new Tuple(*this);
  }
};

// This Special Tuple is to be used only as syntactic sugar, and
// constructed only with the operator `:`, i.e.:
// - passing key-word arguments: func(1, 2, optional_arg:10)
// - slicing lists or strings: my_list[2:10:2] (not implemented)
//
// STuple means one of:
// - Special Tuple, Syntactic Tuple or System Tuple
//
// I haven't decided yet. Suggestions accepted.
class STuple : public Tuple {
 public:
  STuple() { this->type = STUPLE; }
  STuple(const TokenBase* first) {
    this->type = STUPLE;
    list().push_back(packToken(first->clone()));
  }

  STuple(const TokenBase* first, const TokenBase* second) {
    this->type = STUPLE;
    list().push_back(packToken(first->clone()));
    list().push_back(packToken(second->clone()));
  }

 public:
  // Implement the TokenBase abstract class
  TokenBase* clone() const {
    return new STuple(*this);
  }
};

#endif  // OBJECTS_H_
