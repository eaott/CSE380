#include <iostream>
#include "rk4.h"
using namespace Eigen;

VectorXd exponentialDecay(double time, VectorXd y){
  VectorXd ret(y.size());
  ret(0) = y(1);
  ret(1) = - 0.5 * y(1);
  return ret;
}

int main(int argc, char const *argv[]) {
  VectorXd state(2);
  state(0) = 0;
  state(1) = 20;

  for (int i = 0; i < 10000; i++) {
     state = rk4(i * 0.01 - 0.01, state, 0.01, exponentialDecay);
   }
   std::cout << state.transpose() << std::endl;

  return 0;
}
