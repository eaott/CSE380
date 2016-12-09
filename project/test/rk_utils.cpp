#include "catch.hpp"
#define USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "utils.hpp"
#include "rk_utils.hpp"
using namespace Eigen;

void exponentialDecayRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                            double threshold) {
  VectorXd state(1);
  state(0) = 20;

  const int ITER = 10000;
  const double STEP = 0.01;

  for (int i = 0; i < ITER; i++) {
     state = rk(i * STEP, state, STEP, exponentialDecay);
   }
   REQUIRE(equal(state(0), 0.0, threshold));
}
void exponentialDecayRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd))) {
  exponentialDecayRunner(rk, DEFAULT_THRESHOLD);
}

void exponentialDecay2DRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                              double threshold) {
  VectorXd state(2);
  state(0) = 0.0;
  state(1) = 20;

  const int ITER = 10000;
  const double STEP = 0.01;

  for (int i = 0; i < ITER; i++) {
     state = rk(i * STEP, state, STEP, exponentialDecay2D);
   }
   REQUIRE(equal(state(0), 40.0, threshold));
   REQUIRE(equal(state(1), 0.0, threshold));
}
void exponentialDecay2DRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd))) {
  exponentialDecay2DRunner(rk, DEFAULT_THRESHOLD);
}

void sinusoidalRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                      double limit, double xResult, double yResult, double threshold) {
  VectorXd state(2);
  state(0) = 0;
  state(1) = 1;

  const double STEP = 0.0001;
  double theta = 0.0;
  for ( ; theta < limit; theta += STEP) {
     state = rk(theta, state, STEP, sinusoidal);
   }

   // Do the last part of the integral
   state = rk(theta, state, limit - theta, sinusoidal);

   #ifdef DEBUG
    std::cout << limit << " " << state(0) - xResult << " " << state(1) - yResult << std::endl;
   #endif

   REQUIRE(equal(state(0), xResult, threshold));
   REQUIRE(equal(state(1), yResult, threshold));
}
void sinusoidalRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                      double limit, double xResult, double yResult) {
  sinusoidalRunner(rk, limit, xResult, yResult, DEFAULT_THRESHOLD);
}

void magneticRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                    double threshold) {
  VectorXd state(3);
  state(0) = 0;
  state(1) = 20;
  state(2) = -5;

  const double FIXED_V2 = 20 * 20;

  const int ITER = 1000;
  const double STEP = 0.0001;

  double cumulativeError = 0.0;
  double absCumulativeError = 0.0;
  bool zFixed = true;

  for (int i = 0; i < ITER; i++) {
     state = rk(i * STEP, state, STEP, magnetic);
     double v2 = state(0) * state(0) + state(1) * state(1);
     // Include the direction of the error to see if it
     // cancels out.
     cumulativeError = cumulativeError + (v2 - FIXED_V2);

     // Get an idea for how much it moves overall.
     absCumulativeError = absCumulativeError + fabs(v2 - FIXED_V2);

     // Should not change over time at all
     zFixed = zFixed && equal(state(2), -5, 1e-20);
   }
   #ifdef DEBUG
    std::cout << cumulativeError << " " << absCumulativeError << std::endl;
   #endif
   REQUIRE(zFixed);
   REQUIRE(equal(cumulativeError, 0.0, threshold));
   REQUIRE(absCumulativeError < threshold);
}
void magneticRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd))) {
  magneticRunner(rk, 1e-8);
}
