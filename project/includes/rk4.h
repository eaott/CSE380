#ifndef RK4_H
#define RK4_H

#include "Eigen/Dense"


Eigen::VectorXd rk4(double time, Eigen::VectorXd initialState, double step,
             Eigen::VectorXd (*f)(double, Eigen::VectorXd));
Eigen::VectorXd rk38(double time, Eigen::VectorXd initialState, double step,
            Eigen::VectorXd (*f)(double, Eigen::VectorXd));
Eigen::VectorXd rkf(double time, Eigen::VectorXd initialState, double step,
            Eigen::VectorXd (*f)(double, Eigen::VectorXd));

#endif
