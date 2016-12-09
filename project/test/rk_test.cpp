#include "catch.hpp"
#define USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "utils.hpp"
#include "rk.hpp"
#include "rk_utils.hpp"
using namespace Eigen;

// FIXME also need to test problem 1 AT ALL

/*
 * Tests for RK4
 */
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

/*
 * Tests for RK38
 */
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

/*
 * Tests for RKF
 */
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

/*
 * Tests for forward Euler -- these have a lower accuracy by construction
 * so the tests allow for that.
 */
TEST_CASE( "1D exponential decay [euler]" ) {
  exponentialDecayRunner(forwardEuler);
}

TEST_CASE( "1D exponential decay with integral [euler]" ) {
  exponentialDecay2DRunner(forwardEuler);
}

TEST_CASE( "1D sinusoidal - pi / 2 [euler]" ) {
  sinusoidalRunner(forwardEuler, M_PI / 2, 1, 0, 1e-4);
}

TEST_CASE( "1D sinusoidal - pi [euler]" ) {
  sinusoidalRunner(forwardEuler, M_PI, 0, -1, 2e-4);
}

TEST_CASE( "1D sinusoidal - 3 pi / 2 [euler]" ) {
  sinusoidalRunner(forwardEuler, 3 * M_PI / 2, -1, 0, 3e-4);
}

TEST_CASE( "1D sinusoidal - 2 pi [euler]" ) {
  sinusoidalRunner(forwardEuler, 2 * M_PI, 0, 1, 4e-4);
}
