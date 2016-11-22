#include "catch.hpp"
#include <cmath>
#include <iostream>
#include "rk4.h"
using namespace Eigen;

VectorXd exponentialDecay(double time, VectorXd y){
  VectorXd ret(y.size());
  ret(0) = y(1);
  ret(1) = - 0.5 * y(1);
  return ret;
}

bool equal(double a, double b) {
  return fabs(a - b) < 1e-10;
}

TEST_CASE( "1D exponential decay", "[rk4]" ) {
  VectorXd state(2);
  state(0) = 0;
  state(1) = 20;

  for (int i = 0; i < 10000; i++) {
     state = rk4(i * 0.01 - 0.01, state, 0.01, exponentialDecay);
   }
   std::cout << state(0) - 40.0 << std::endl;
   REQUIRE(equal(state(0), 40.0));
   REQUIRE(equal(state(1), 0.0));
}
