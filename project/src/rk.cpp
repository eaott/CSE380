/*
 * Custom Runge-Kutta (and Euler) methods of integration.
 * In all code for the problems given, the problems have
 * ODEs with no explicit time dependence, but I thought
 * it would be best to provide for it.
 */
#include "rk.hpp"
using namespace Eigen;

VectorXd rk4(double t, VectorXd initialState, double step, Rk_Function f) {
  // Standard Runge-Kutta
  VectorXd k1 = step * f(t, initialState);
  VectorXd k2 = step * f(t + 0.5 * step, initialState + 0.5 * k1);
  VectorXd k3 = step * f(t + 0.5 * step, initialState + 0.5 * k2);
  VectorXd k4 = step * f(t + step, initialState + k3);
  return initialState + 1.0/6.0 * (k1 + k2 * 2 + k3 * 2 + k4);
}

VectorXd rk38(double t, VectorXd initialState, double step, Rk_Function f) {
  // see https://en.wikipedia.org/wiki/List_of_Runge%E2%80%93Kutta_methods#3.2F8-rule_fourth-order_method
  VectorXd k1 = step * f(t, initialState);
  VectorXd k2 = step * f(t + step / 3.0, initialState + k1 / 3.0);
  VectorXd k3 = step * f(t + 2.0 * step / 3.0, initialState - k1 / 3.0 + k2);
  VectorXd k4 = step * f(t + step, initialState + k1 - k2 + k3);
  return initialState + 1.0/8.0 * (k1 + 3 * k2 + 3 * k3 + k4);
}

VectorXd rkf(double t, VectorXd initialState, double step, Rk_Function f) {
  // see https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method
  VectorXd k1 = step * f(t, initialState);
  VectorXd k2 = step * f(t + step / 4.0, initialState + k1 / 4.0);
  VectorXd k3 = step * f(t + step * 3.0 / 8.0,
                         initialState + k1 * 3.0 / 32.0 + k2 * 9.0 / 32.0);
  VectorXd k4 = step * f(t + step * 12.0 / 13.0,
                         initialState + k1 * 1932.0 / 2197.0 - k2 * 7200.0 / 2197.0 + k3 * 7296.0 / 2197.0);
  VectorXd k5 = step * f(t + step,
                         initialState + k1 * 439.0 / 216.0 - k2 * 8.0 + k3 * 3680.0 / 513.0 - k4 * 845.0 / 4104.0);
  return initialState + k1 * 25.0 / 216.0 + k3 * 1408.0 / 2565.0 + k4 * 2197.0 / 4104.0 - k5 / 5.0;
}

VectorXd forwardEuler(double t, VectorXd initialState, double step, Rk_Function f) {
  // Simple forward Euler in same format as the RK methods
  VectorXd k1 = step * f(t, initialState);
  return initialState + k1;
}
