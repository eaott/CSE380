#ifndef UTILS
#define UTILS
#define DEFAULT_THRESHOLD 1e-10
#include "Eigen/Dense"
using namespace Eigen;

bool equal(double a, double b, double threshold);

bool equal(double a, double b);

VectorXd exponentialDecay(double time, VectorXd y);

VectorXd exponentialDecay2D(double time, VectorXd y);

VectorXd sinusoidal(double time, VectorXd y);

VectorXd magnetic(double time, VectorXd y);
#endif
