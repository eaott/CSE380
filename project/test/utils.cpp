#include "utils.hpp"
#include "rk4_utils.hpp"
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
