#ifndef RK_H
#define RK_H

#include "Eigen/Dense"
using namespace Eigen;
typedef VectorXd (*Rk_Function) (double, VectorXd);
typedef VectorXd (*Rk_Method) (double, VectorXd, double, Rk_Function);

VectorXd rk4(double time, VectorXd initialState, double step,
             Rk_Function f);
VectorXd rk38(double time, VectorXd initialState, double step,
            Rk_Function f);
VectorXd rkf(double time, VectorXd initialState, double step,
            Rk_Function f);
VectorXd forwardEuler(double time, VectorXd initialState, double step,
            Rk_Function f);

#endif
