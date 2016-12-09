#ifndef RK_H
#define RK_H

#include "Eigen/Dense"
using namespace Eigen;

// Signature of function that takes in a time as a double,
// state as a VectorXd and outputs another VectorXd of the new state.
// All problems should have this signature.
typedef VectorXd (*Rk_Function) (double, VectorXd);
// Signature of the below solvers. They take the current
// state (time, state), the step size in time to integrate, and the
// Rk_Function for the specific problem, then return the new state.
typedef VectorXd (*Rk_Method) (double, VectorXd, double, Rk_Function);

// standard Runge-Kutta
VectorXd rk4(double t, VectorXd initialState, double step,
             Rk_Function f);

// "3/8" rule Runge-Kutta
VectorXd rk38(double t, VectorXd initialState, double step,
            Rk_Function f);

// Runge-Kutta-Fehlberg
VectorXd rkf(double t, VectorXd initialState, double step,
            Rk_Function f);

// forward Euler
VectorXd forwardEuler(double t, VectorXd initialState, double step,
            Rk_Function f);

#endif
