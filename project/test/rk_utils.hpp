#ifndef RK_UTILS
#define RK_UTILS

#include "utils.hpp"
#include "rk_utils.hpp"
using namespace Eigen;

void exponentialDecayRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                            double threshold);
void exponentialDecayRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)));

void exponentialDecay2DRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                              double threshold);
void exponentialDecay2DRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)));

void sinusoidalRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                      double limit, double xResult, double yResult, double threshold);
void sinusoidalRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                      double limit, double xResult, double yResult);

void magneticRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)),
                    double threshold);
void magneticRunner(VectorXd (*rk)(double, VectorXd, double, VectorXd (double, VectorXd)));

#endif
