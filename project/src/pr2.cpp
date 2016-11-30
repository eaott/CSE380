#include <iostream>
#include <fstream>
#include "rk.hpp"
#include "INIReader.h"
using std::cout;
using std::endl;
#include "H5Cpp.h"
#include <ctime>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

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

typedef VectorXd (*Rk_Method) (double, VectorXd, double, Rk_Function);


int gsl_particle(double t, const double y[], double f[], void * params) {
  f[idxX] = y[idxUx];
  f[idxY] = y[idxUy];
  f[idxZ] = f[idxUz];
  f[idxUx] = OMEGA * f[idxUy] - TAU_INV * y[idxUx];
  f[idxUy] = -OMEGA * f[idxUx] - TAU_INV * y[idxUy];
  f[idxUz] = - TAU_INV * f[idxUz];
  return GSL_SUCCESS;
}

int gls_jacobian(double t, const double y[], double *dfdy, double dfdt[], void * params) {
  gsl_matrix_view dfdy_mat
    = gsl_matrix_view_array (dfdy, 6, 6);
  gsl_matrix * m = &dfdy_mat.matrix;
  for (int i = 0; i < 6; i++) {
    // No explicit time-dependence.
    dfdt[i] = 0.0;
    for (int j = 0; j < 6; j++) {
      gsl_matrix_set (m, i, j, 0.0);
    }
  }
  gsl_matrix_set (m, idxX, idxUx, 1.0);

  gsl_matrix_set (m, idxY, idxUy, 1.0);

  gsl_matrix_set (m, idxZ, idxUz, 1.0);

  gsl_matrix_set (m, idxUx, idxUx, -TAU_INV);
  gsl_matrix_set (m, idxUx, idxUy, OMEGA);

  gsl_matrix_set (m, idxUy, idxUx, -OMEGA);
  gsl_matrix_set (m, idxUy, idxUy, -TAU_INV);

  gsl_matrix_set (m, idxUz, idxUz, -TAU_INV);

  return GSL_SUCCESS;
}

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

void writeAttrs(DataSet dataset, int iter, double step, string method, string time_str) {
  DataSpace attr_dataspace = DataSpace(H5S_SCALAR);
  StrType strdatatype(PredType::C_S1, 256);
  Attribute attribute = dataset.createAttribute("Main data info", strdatatype, attr_dataspace);
  attribute.write(strdatatype, "rows are in the order (x, y, z)");

  Attribute iterAttr = dataset.createAttribute("iter", PredType::STD_I32BE, attr_dataspace);
  iterAttr.write(PredType::NATIVE_INT, &iter);

  Attribute stepAttr = dataset.createAttribute("step", PredType::IEEE_F32BE, attr_dataspace);
  stepAttr.write(PredType::NATIVE_DOUBLE, &step);

  Attribute methodAttr = dataset.createAttribute("method", strdatatype, attr_dataspace);
  methodAttr.write(strdatatype, method);

  Attribute timeAttr = dataset.createAttribute("timestamp", strdatatype, attr_dataspace);
  timeAttr.write(strdatatype, time_str);
}

std::string getTime() {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,80,"%Y_%m_%d__%I_%M_%S",timeinfo);
  std::string str(buffer);
  return str;
}

int rk_gsl(int dim, double y[], double **output,
           double step, double iter, double startT, std::string methodStr) {
  double epsilon_abs = 0.01;
  double epsilon_rel = 0.01;
  double t = startT;

  gsl_odeiv2_system sys = {gsl_particle, gls_jacobian, dim, NULL};

  const gsl_odeiv2_step_type * method;

  if (methodStr == "rkf") {
    method = gsl_odeiv2_step_rkf45;
  } else if (methodStr == "rk8pd") {
    method = gsl_odeiv2_step_rk8pd;
  } else {
    method = gsl_odeiv2_step_rk4;
  }

  gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new(&sys, method,
                                    step, epsilon_abs, epsilon_rel);
  for (int i = 0; i < iter; i++) {
    int status = gsl_odeiv2_driver_apply_fixed_step (d, &t, step, iter, y);
    if (status != GSL_SUCCESS) {
      std::cout << "Failure within GSL" << std::endl;
      return 1;
    }
    output[i][idxX] = y[idxX];
    output[i][idxY] = y[idxY];
    output[i][idxZ] = y[idxZ];
  }
  return 0;
}

int run_pr2(INIReader reader) {
  int iter = reader.GetInteger("problem2", "iter", 10000);
  if (iter <= 0) {
    std::cout << "Should have a positive number of iterations." << std::endl;
    return 1;
  }
  double step = reader.GetReal("problem2", "step", 0.01);
  if (step <= 0) {
    std::cout << "Should have a positive step size." << std::endl;
    return 1;
  }
  int skip = reader.GetInteger("problem2", "skip", 1);
  if (skip <= 0) {
    std::cout << "Should have a positive skip size." << std::endl;
  }
  string methodStr = reader.Get("problem2", "method", "evan-rk4");

  bool verification = reader.GetBoolean("problem2", "verification", "false");

#ifdef DEBUG
  std::cout << iter << " " << step << " " << methodStr << std::endl;
#endif
  std::string runTime = getTime();

  VectorXd state(6);
  state(idxX) = 0;
  state(idxY) = 0;
  state(idxZ) = 0;

  state(idxUx) = 20;
  state(idxUy) = 0;
  state(idxUz) = 2;

  double data[3][iter];

  // FIXME: here, should actually have evan-rk38, etc. along with the GSL
  // versions, as done in pr1.cpp. Let's separate the runners
  // into a GSL and Evan version, passing the parsed reader...
  if (method.substr(0, 4) == "evan") {
    if (methodStr == "evan-rk38") {
      rk = rk38;
    } else if (methodStr == "evan-rkf") {
      rk = rkf;
    } else {
      methodStr = "evan-rk4";
      rk = rk4;
    }
    Rk_Method rk;
    for (int i = 0; i < iter; i++) {
      state = rk(i * step, state, step, particleInField);
      data[idxX][i] = state(idxX);
      data[idxY][i] = state(idxY);
      data[idxZ][i] = state(idxZ);
    }
  } else {
    double y[6];
    for (int i = 0; i < 6; i++) { y[i] = state(i); }

    int status = rk_gsl(6, y, data,
               h, iter, 0.0, methodStr);
    if (status != 0) {
      return status;
    }
  }



  // FIXME consider adding a "skip" variable to not include
  // the entire data set in the hdf5 file, but maybe every 10th point
  H5File * file = new H5File("file.h5", H5F_ACC_TRUNC);

  hsize_t dataset_dims[2] = {3, iter};
  hsize_t chunk_dims[2] = {1, 1000};
  DSetCreatPropList  *plist = new  DSetCreatPropList;
	plist->setChunk(2, chunk_dims);
	plist->setDeflate(6);

  DataSpace dataspace(2, dataset_dims);
  DataSet* dataset = new DataSet(file->createDataSet(
    "dataset", PredType::NATIVE_DOUBLE, dataspace, *plist
  ));
  dataset->write(data, PredType::NATIVE_DOUBLE);
  writeAttrs(*dataset, iter, step, methodStr, runTime);

  // If in verification mode, include output from analytical
  // solution.
  if (verification) {
    VectorXd analytical_value(6);
    double analytical_data[3][iter];
    for (int i = 0; i < iter; i++) {
      analytical_value = analytical((i + 1) * step);
      analytical_data[idxX][i] = analytical_value(idxX);
      analytical_data[idxY][i] = analytical_value(idxY);
      analytical_data[idxZ][i] = analytical_value(idxZ);
    }
    DataSpace analytical_dataspace(2, dataset_dims);
    DataSet* analytical_dataset = new DataSet(file->createDataSet(
      "analytical", PredType::NATIVE_DOUBLE, analytical_dataspace, *plist
    ));
    analytical_dataset->write(analytical_data, PredType::NATIVE_DOUBLE);
    writeAttrs(*analytical_dataset, iter, step, methodStr, runTime);
    delete analytical_dataset;
  }

  delete plist;
  delete dataset;
  delete file;
  return 0;
}
