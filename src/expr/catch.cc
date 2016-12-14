#define CATCH_CONFIG_RUNNER
#include "catch.h"

// Defined on test-shunting-yard.cpp
void PREPARE_ENVIRONMENT();

int main(int argc, char** argv) {
  PREPARE_ENVIRONMENT();
  return Catch::Session().run(argc, argv);
}
