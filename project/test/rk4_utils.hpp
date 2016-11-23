#ifndef RK4_UTILS
#define RK4_UTILS

#include "utils.hpp"
#include "rk4_utils.hpp"
using namespace Eigen;

void exponentialDecayRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)));

void exponentialDecay2DRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)));

void sinusoidalRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                      double limit, double xResult, double yResult);

void magneticRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)));

#endif
