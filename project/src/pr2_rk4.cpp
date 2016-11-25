#include <iostream>
#include <fstream>
#include "rk4.hpp"
#include "INIReader.h"
using std::cout;
using std::endl;
#include "H5Cpp.h"

using namespace H5;
using namespace std;
using namespace Eigen;

double TAU = 5.0;
double TAU_INV = 1.0 / TAU;
double OMEGA = 5.0;

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

VectorXd analytical(double time) {
  VectorXd ret(6);
  ret(idxUx) = 20 * exp(-time * TAU_INV) * cos(time * OMEGA);
  ret(idxUy) = -20 * exp(-time * TAU_INV) * sin(time * OMEGA);
  ret(idxUz) = 2 * exp(-time * TAU_INV);
  ret(idxX) =
    (20 * TAU - TAU * ret(idxUx) - TAU * TAU * OMEGA * ret(idxUy))
    /
    (1 + TAU * TAU * OMEGA * OMEGA);
  ret(idxY) =
    (-20 * TAU * TAU * OMEGA + TAU * TAU * OMEGA * ret(idxUx) - TAU * ret(idxUy))
    /
    (1 + TAU * TAU * OMEGA * OMEGA);
  ret(idxZ) = TAU * (2 - ret(idxUz));
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

  VectorXd state(6);
  state(idxX) = 0;
  state(idxY) = 0;
  state(idxZ) = 0;

  state(idxUx) = 20;
  state(idxUy) = 0;
  state(idxUz) = 2;


  VectorXd analytical_value(6);
  double data[3][iter];
  double analytical_data[3][iter];
  for (int i = 0; i < iter; i++) {
    state = rkf(i * step, state, step, particleInField);
    data[idxX][i] = state(idxX);
    data[idxY][i] = state(idxY);
    data[idxZ][i] = state(idxZ);

    analytical_value = analytical((i + 1) * step);
    analytical_data[idxX][i] = analytical_value(idxX);
    analytical_data[idxY][i] = analytical_value(idxY);
    analytical_data[idxZ][i] = analytical_value(idxZ);
  }

  H5File * file = new H5File("file.h5", H5F_ACC_TRUNC);

  hsize_t dataset_dims[2];
  dataset_dims[0] = 3;
  dataset_dims[1] = iter;
  DataSpace dataspace(2, dataset_dims);
  DataSet* dataset = new DataSet(file->createDataSet(
    "dataset", PredType::NATIVE_DOUBLE, dataspace
  ));
  dataset->write(data, PredType::NATIVE_DOUBLE);

  DataSpace attr_dataspace = DataSpace(H5S_SCALAR);
  StrType strdatatype(PredType::C_S1, 256);
  Attribute attribute = dataset->createAttribute("Rows", strdatatype, attr_dataspace);
  attribute.write(strdatatype, "rows are in the order (x, y, z)");

  DataSpace analytical_dataspace(2, dataset_dims);
  DataSet* analytical_dataset = new DataSet(file->createDataSet(
    "analytical", PredType::NATIVE_DOUBLE, analytical_dataspace
  ));
  analytical_dataset->write(analytical_data, PredType::NATIVE_DOUBLE);

  Attribute analytical_attribute = analytical_dataset->createAttribute("Rows", strdatatype, attr_dataspace);
  analytical_attribute.write(strdatatype, "rows are in the order (x, y, z)");

  delete dataset;
  delete file;
  return 0;
}
