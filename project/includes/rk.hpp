#ifndef RK_H
#define RK_H

#include "Eigen/Dense"
using namespace Eigen;

VectorXd rk4(double time, VectorXd initialState, double step,
             VectorXd (*f)(double, VectorXd));
VectorXd rk38(double time, VectorXd initialState, double step,
            VectorXd (*f)(double, VectorXd));
VectorXd rkf(double time, VectorXd initialState, double step,
            VectorXd (*f)(double, VectorXd));

#endif
