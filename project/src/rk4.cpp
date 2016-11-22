#include "rk4.h"
using namespace Eigen;

// We know that it's independent of time, but the code shouldn't
// need to know that necessarily to have a nice interface.
VectorXd rk4(double time, VectorXd initialState, double step, VectorXd (*f)(double, VectorXd)) {
  VectorXd k1 = step * f(time, initialState);
  VectorXd k2 = step * f(time + 0.5 * step, initialState + 0.5 * k1);
  VectorXd k3 = step * f(time + 0.5 * step, initialState + 0.5 * k2);
  VectorXd k4 = step * f(time + step, initialState + k3);
  return initialState + 1.0/6.0 * (k1 + k2 * 2 + k3 * 2 + k4);
}
