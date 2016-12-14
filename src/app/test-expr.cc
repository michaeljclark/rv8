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
