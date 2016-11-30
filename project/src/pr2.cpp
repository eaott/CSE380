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
#define VECTOR_SIZE 6

typedef VectorXd (*Rk_Method) (double, VectorXd, double, Rk_Function);

int gsl_particle(double t, const double y[], double f[], void * params) {
  f[idxX] = y[idxUx];
  f[idxY] = y[idxUy];
  f[idxZ] = y[idxUz];
  f[idxUx] = OMEGA * y[idxUy] - TAU_INV * y[idxUx];
  f[idxUy] = - OMEGA * y[idxUx] - TAU_INV * y[idxUy];
  f[idxUz] = - TAU_INV * y[idxUz];
  return GSL_SUCCESS;
}

VectorXd particleInField(double time, VectorXd y){
  VectorXd ret(VECTOR_SIZE);
  // Ensure the implementations are identical.
  gsl_particle(time, y.data(), ret.data(), NULL);
  return ret;
}

// FIXME Can I move the GSL functions somewhere else?
// FIXME Can I unit test on seeing if my implementation and GSL's match for a single iteration?
// FIXME Adding comments everywhere
// FIXME maybe making one GSL runner function that can be used by both problem 1 and 2 -- allowing rk4, rkf45, euler, etc. as the options
// FIXME do same thing but with my implementation? mostly there already
// FIXME so, idea is having src/GSL.cpp src/solvers.cpp and src/models.cpp or something

int gls_jacobian(double t, const double y[], double *dfdy, double dfdt[], void * params) {
  gsl_matrix_view dfdy_mat
    = gsl_matrix_view_array (dfdy, VECTOR_SIZE, VECTOR_SIZE);
  gsl_matrix * m = &dfdy_mat.matrix;
  for (int i = 0; i < VECTOR_SIZE; i++) {
    // No explicit time-dependence.
    dfdt[i] = 0.0;
    for (int j = 0; j < VECTOR_SIZE; j++) {
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

VectorXd analytical(double time) {
  VectorXd ret(VECTOR_SIZE);
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

MatrixXd rk_gsl(double y[], int iter,
           double step, double startT,
           double epsilon_abs, double epsilon_rel, std::string methodStr) {
  double t = startT;
  MatrixXd output(3, iter);

  gsl_odeiv2_system sys = {gsl_particle, gls_jacobian, VECTOR_SIZE, NULL};

  const gsl_odeiv2_step_type * method;

  if (methodStr == "rkf") {
    method = gsl_odeiv2_step_rkf45;
  } else if (methodStr == "rk8pd") {
    method = gsl_odeiv2_step_rk8pd;
  } else {
    method = gsl_odeiv2_step_rk4;
  }

  gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new(
      &sys, method, step, epsilon_abs, epsilon_rel);

  for (int i = 0; i < iter; i++) {
    // Go one step at a time, so we can save the intermediate states.
    int status = gsl_odeiv2_driver_apply_fixed_step (
        d, &t, step, 1, y);

    if (status != GSL_SUCCESS) {
      std::cout << "Failure within GSL" << std::endl;
      break;
    }
    output(idxX, i) = y[idxX];
    output(idxY, i) = y[idxY];
    output(idxZ, i) = y[idxZ];
  }
  return output;
}

MatrixXd rk_evan(VectorXd state, int iter,
           double step, double startT,
           std::string methodStr) {
   MatrixXd output(3, iter);
   Rk_Method rk;

   if (methodStr == "evan-rk38") {
     rk = rk38;
   } else if (methodStr == "evan-rkf") {
     rk = rkf;
   } else {
     rk = rk4;
   }
   VectorXd tempState = state;
   for (int i = 0; i < iter; i++) {
     tempState = rk(i * step, tempState, step, particleInField);
     output(idxX, i) = tempState(idxX);
     output(idxY, i) = tempState(idxY);
     output(idxZ, i) = tempState(idxZ);
   }
   return output;
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
  double epsilon_abs = reader.GetReal("problem2", "epsilon_abs", 1e-6);
  double epsilon_rel = reader.GetReal("problem2", "epsilon_rel", 0.0);


#ifdef DEBUG
  std::cout << "iter:" << iter << "\nstep:" << step <<
               "\nmethod:" << methodStr << "\nskip:" << skip <<
               "\nverification:" << verification <<
               "\nepsilon_abs:" << epsilon_abs <<
               "\nepsilon_rel:" << epsilon_rel << std::endl;
#endif
  std::string runTime = getTime();

  VectorXd state(VECTOR_SIZE);
  state(idxX) = 0;
  state(idxY) = 0;
  state(idxZ) = 0;

  state(idxUx) = 20;
  state(idxUy) = 0;
  state(idxUz) = 2;

  MatrixXd data(3, iter);

  // FIXME: here, should actually have evan-rk38, etc. along with the GSL
  // versions, as done in pr1.cpp. Let's separate the runners
  // into a GSL and Evan version, passing the parsed reader...
  if (methodStr.substr(0, 4) == "evan") {
#ifdef DEBUG
  std::cout << "Using Evan's methods:" << methodStr << std::endl;
#endif
    data = rk_evan(state, iter, step, 0.0, methodStr);
  } else {
#ifdef DEBUG
  std::cout << "Using GSL's methods:" << methodStr << std::endl;
#endif
    data = rk_gsl(state.data(), iter,
                        step, 0.0, epsilon_abs, epsilon_rel, methodStr);
  }

#ifdef DEBUG
  std::cout << "data(0,0): "<< data(0,0) << std::endl;
#endif

  // FIXME consider adding a "skip" variable to not include
  // the entire data set in the hdf5 file, but maybe every 10th point
  H5File * file = new H5File("file.h5", H5F_ACC_TRUNC);

  hsize_t dataset_dims[2] = {3, iter};
  DSetCreatPropList  *plist = new  DSetCreatPropList;
  if (iter > 1000) {
    hsize_t chunk_dims[2] = {1, 1000};
  	plist->setChunk(2, chunk_dims);
    // Arbitrarily chosen level of compression
  	plist->setDeflate(6);
  }
  DataSpace dataspace(2, dataset_dims);
  DataSet* dataset = new DataSet(file->createDataSet(
    "dataset", PredType::NATIVE_DOUBLE, dataspace, *plist
  ));
  double hdf5_data[3][iter];
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < iter; j++)
      hdf5_data[i][j] = data(i, j);
  dataset->write(hdf5_data, PredType::NATIVE_DOUBLE);
  writeAttrs(*dataset, iter, step, methodStr, runTime);

  // If in verification mode, include output from analytical
  // solution.
  if (verification) {
    VectorXd analytical_value(VECTOR_SIZE);
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
