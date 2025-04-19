#include "../include/app.h"
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>


unsigned int Factorial( unsigned int number ) {
  return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE("factorials are computed", "[factorial]") {
  REQUIRE( Factorial(1) == 1 );
  REQUIRE( Factorial(2) == 2 );
  REQUIRE( Factorial(3) == 6 );
  REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE("Hello", "[hello]") {
  spdlog::info("Hello");
}

TEST_CASE("App init", "[app]") {
  App *app = new App();
  REQUIRE(app->init());
  REQUIRE(app->getRenderer() != nullptr);
  REQUIRE(app->getWindow() != nullptr);
}

TEST_CASE("App run", "[app]") {
}
