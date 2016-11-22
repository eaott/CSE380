#include <iostream>
#include <fstream>
#include "Eigen/Dense"
using namespace std;
using namespace Eigen;

double TAU = 5;
double TAU_INV = 1 / TAU;
double OMEGA = 5;

#define idxX 0
#define idxY 1
#define idxZ 2
#define idxUx 3
#define idxUy 4
#define idxUz 5


VectorXd particleInField(double time, VectorXd y){
  VectorXd ret(y.size());
  ret(idxX) = y(idxUx);
  ret(idxY) = y(idxUy);
  ret(idxZ) = y(idxUz);
  ret(idxUx) = OMEGA * y(idxUy) - TAU_INV * y(idxUx);
  ret(idxUy) = -OMEGA * y(idxUx) - TAU_INV * y(idxUy);
  ret(idxUz) = - TAU_INV * y(idxUz);
  return ret;
}

// We know that it's independent of time, but the code shouldn't
// need to know that necessarily to have a nice interface.
VectorXd rk4(double time, VectorXd initialState, double step, VectorXd (*f)(double, VectorXd)) {
  VectorXd k1 = step * f(time, initialState);
  VectorXd k2 = step * f(time + 0.5 * step, initialState + 0.5 * k1);
  VectorXd k3 = step * f(time + 0.5 * step, initialState + 0.5 * k2);
  VectorXd k4 = step * f(time + step, initialState + k3);
  return initialState + 1.0/6.0 * (k1 + k2 * 2 + k3 * 2 + k4);
}

int main(int argc, char const *argv[]) {
  VectorXd state(6);
  state(idxX) = 0;
  state(idxY) = 0;
  state(idxZ) = 0;
  state(idxUx) = 20;
  state(idxUy) = 0;
  state(idxUz) = 2;
  ofstream out;
  out.open("test.csv");
  out << "x,y,z\n";
  out << state(0) << "," << state(1) << "," << state(2) << "\n";
  for (int i = 0; i < 10000; i++) {
     state = rk4(i * 0.01 - 0.01, state, 0.01, particleInField);
     out << state(0) << "," << state(1) << "," << state(2) << "\n";
   }
   out.close();

  return 0;
}
