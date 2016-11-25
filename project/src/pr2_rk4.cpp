#include <iostream>
#include <fstream>
#include "rk4.hpp"
#include "INIReader.h"
using std::cout;
using std::endl;
#include "H5Cpp.h"
#include "eigen3-hdf5.hpp"

using namespace H5;
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

int main(int argc, char const *argv[]) {
  INIReader reader("test.ini");
  if (reader.ParseError() < 0) {
    std::cout << "Can't load test.ini\n" << std::endl;
    return 1;
  }
  int iter = reader.GetInteger("problem2", "iter", 10000);
  double step = reader.GetReal("problem2", "step", 0.01);
  std::cout << iter << " " << step << std::endl;

  // FIXME: examples of syntax...
  std::cout << "Config loaded from 'test.ini': version="
           << reader.GetInteger("protocol", "version", -1) << ", name="
           << reader.Get("user", "name", "UNKNOWN") << ", email="
           << reader.Get("user", "email", "UNKNOWN") << ", pi="
           << reader.GetReal("user", "pi", -1) << ", active="
           << reader.GetBoolean("user", "active", true) << "\n";


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

  VectorXd x(iter);
  VectorXd y(iter);
  VectorXd z(iter);
  for (int i = 0; i < iter; i++) {
    state = rk4(i * step, state, step, particleInField);
    x(i) = state(idxX);
    y(i) = state(idxY);
    z(i) = state(idxZ);
    out << state(0) << "," << state(1) << "," << state(2) << "\n";
  }
  out.close();

  H5File * file = new H5File("file.h5", H5F_ACC_TRUNC);
  EigenHDF5::save(*file, "x", x);
  EigenHDF5::save(*file, "y", y);
  EigenHDF5::save(*file, "z", z);
  delete file;
  return 0;
}
