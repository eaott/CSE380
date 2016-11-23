#include "catch.hpp"
#define USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "rk4.h"
using namespace Eigen;

bool equal(double a, double b, double threshold) {
  return fabs(a - b) < threshold;
}

bool equal(double a, double b) {
  return equal(a, b, 1e-10);
}

VectorXd exponentialDecay(double time, VectorXd y){
  VectorXd ret(y.size());
  ret(0) = - 0.5 * y(0);
  return ret;
}

VectorXd exponentialDecay2D(double time, VectorXd y){
  VectorXd ret(y.size());
  ret(0) = y(1);
  ret(1) = - 0.5 * y(1);
  return ret;
}

VectorXd sinusoidal(double time, VectorXd y){
  VectorXd ret(y.size());
  ret(0) = y(1);
  ret(1) = - y(0);
  return ret;
}

VectorXd magnetic(double time, VectorXd y){
  const double B0 = 5;
  VectorXd ret(y.size());
  ret(0) = B0 * y(1);
  ret(1) = - B0 * y(0);
  ret(2) = 0;
  return ret;
}

void exponentialDecayRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd))) {
  VectorXd state(1);
  state(0) = 20;

  const int ITER = 10000;
  const double STEP = 0.01;

  for (int i = 0; i < ITER; i++) {
     state = rk(i * STEP, state, STEP, exponentialDecay);
   }
   REQUIRE(equal(state(0), 0.0));
}

void exponentialDecay2DRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd))) {
  VectorXd state(2);
  state(0) = 0.0;
  state(1) = 20;

  const int ITER = 10000;
  const double STEP = 0.01;

  for (int i = 0; i < ITER; i++) {
     state = rk(i * STEP, state, STEP, exponentialDecay2D);
   }
   REQUIRE(equal(state(0), 40.0));
   REQUIRE(equal(state(1), 0.0));
}


void sinusoidalRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                      double limit, double xResult, double yResult) {
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

   REQUIRE(equal(state(0), xResult));
   REQUIRE(equal(state(1), yResult));
}

void magneticRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd))) {
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
     state = rk4(i * STEP, state, STEP, magnetic);
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
   REQUIRE(equal(cumulativeError, 0.0, 1e-9));
   REQUIRE(absCumulativeError < 1e-8);
}

TEST_CASE( "1D exponential decay [rk4]" ) {
  exponentialDecayRunner(rk4);
}

TEST_CASE( "1D exponential decay with integral [rk4]" ) {
  exponentialDecay2DRunner(rk4);
}

TEST_CASE( "1D sinusoidal - pi / 2 [rk4]" ) {
  sinusoidalRunner(rk4, M_PI / 2, 1, 0);
}

TEST_CASE( "1D sinusoidal - pi [rk4]" ) {
  sinusoidalRunner(rk4, M_PI, 0, -1);
}

TEST_CASE( "1D sinusoidal - 3 pi / 2 [rk4]" ) {
  sinusoidalRunner(rk4, 3 * M_PI / 2, -1, 0);
}

TEST_CASE( "1D sinusoidal - 2 pi [rk4]" ) {
  sinusoidalRunner(rk4, 2 * M_PI, 0, 1);
}

TEST_CASE( "3D particle in magnetic field [rk4]" ) {
  magneticRunner(rk4);
}







TEST_CASE( "1D exponential decay [rk38]" ) {
  exponentialDecayRunner(rk38);
}

TEST_CASE( "1D exponential decay with integral [rk38]" ) {
  exponentialDecay2DRunner(rk38);
}

TEST_CASE( "1D sinusoidal - pi / 2 [rk38]" ) {
  sinusoidalRunner(rk38, M_PI / 2, 1, 0);
}

TEST_CASE( "1D sinusoidal - pi [rk38]" ) {
  sinusoidalRunner(rk38, M_PI, 0, -1);
}

TEST_CASE( "1D sinusoidal - 3 pi / 2 [rk38]" ) {
  sinusoidalRunner(rk38, 3 * M_PI / 2, -1, 0);
}

TEST_CASE( "1D sinusoidal - 2 pi [rk38]" ) {
  sinusoidalRunner(rk38, 2 * M_PI, 0, 1);
}

TEST_CASE( "3D particle in magnetic field [rk38]" ) {
  magneticRunner(rk38);
}





TEST_CASE( "1D exponential decay [rkf]" ) {
  exponentialDecayRunner(rkf);
}

TEST_CASE( "1D exponential decay with integral [rkf]" ) {
  exponentialDecay2DRunner(rkf);
}

TEST_CASE( "1D sinusoidal - pi / 2 [rkf]" ) {
  sinusoidalRunner(rkf, M_PI / 2, 1, 0);
}

TEST_CASE( "1D sinusoidal - pi [rkf]" ) {
  sinusoidalRunner(rkf, M_PI, 0, -1);
}

TEST_CASE( "1D sinusoidal - 3 pi / 2 [rkf]" ) {
  sinusoidalRunner(rkf, 3 * M_PI / 2, -1, 0);
}

TEST_CASE( "1D sinusoidal - 2 pi [rkf]" ) {
  sinusoidalRunner(rkf, 2 * M_PI, 0, 1);
}

TEST_CASE( "3D particle in magnetic field [rkf]" ) {
  magneticRunner(rkf);
}
