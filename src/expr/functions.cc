#include "./shunting-yard.h"
#include "./functions.h"
#include "./shunting-yard-exceptions.h"

/* * * * * class Function * * * * */
packToken Function::call(packToken _this, Function* func,
                         TokenList* args, TokenMap scope) {
  // Build the local namespace:
  TokenMap kwargs;
  TokenMap local = scope.getChild();

  args_t arg_names = func->args();

  TokenList_t::iterator args_it = args->list().begin();
  args_t::const_iterator names_it = arg_names.begin();

  /* * * * * Parse named arguments: * * * * */

  while (args_it != args->list().end() && names_it != arg_names.end()) {
    // If the positional argument list is over:
    if ((*args_it)->type == STUPLE) break;

    // Else add it to the local namespace:
    local[*names_it] = *args_it;

    ++args_it;
    ++names_it;
  }

  /* * * * * Parse extra positional arguments: * * * * */

  TokenList arglist;
  for (; args_it != args->list().end(); ++args_it) {
    // If there is a keyword argument:
    if ((*args_it)->type == STUPLE) break;
    // Else add it to arglist:
    arglist.list().push_back(*args_it);
  }

  /* * * * * Parse keyword arguments: * * * * */

  for (; args_it != args->list().end(); ++args_it) {
    packToken& arg = *args_it;

    if (arg->type != STUPLE) {
      throw syntax_error("Positional argument follows keyword argument");
    }

    STuple* st = static_cast<STuple*>(arg.token());

    if (st->list().size() != 2) {
      throw syntax_error("Keyword tuples must have exactly 2 items!");
    }

    if (st->list()[0]->type != STR) {
      throw syntax_error("Keyword first argument should be of type string!");
    }

    // Save it:
    std::string key = st->list()[0].asString();
    packToken& value = st->list()[1];
    kwargs[key] = value;
  }

  /* * * * * Set missing arguments to None: * * * * */

  for (; names_it != arg_names.end(); ++names_it) {
    // If not set by a keyword argument:
    if (local.map().count(*names_it) == 0) {
      local[*names_it] = packToken::None;
    }
  }

  /* * * * * Set built-in variables: * * * * */

  local["this"] = _this;
  local["args"] = arglist;
  local["kwargs"] = kwargs;

  return func->exec(local);
}
