/*
 * Class that handles Problem 2.
 * In particular, solves the more complicated ODE below according to configuration
 * options in an INIReader. Writes trajectory at each step to an HDF5 file.
 */

#include <iostream>
#include <fstream>
#include "rk.hpp"
#include "INIReader.h"
using std::cout;
using std::endl;
#include "H5Cpp.h"
#include "pr2.hpp"
#include <ctime>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

using namespace H5;
using namespace std;
using namespace Eigen;

// These are standard assumptions for the problem
double TAU = 5.0;
double TAU_INV = 1.0 / TAU;
double OMEGA = 5.0;
// And the initial state at t = 0.
double X0 = 0;
double Y0 = 0;
double Z0 = 0;
double UX0 = 20;
double UY0 = 0;
double UZ0 = 2;

// Useful for not having as many magic numbers in the code.
// These three are for the position
#define idxX 0
#define idxY 1
#define idxZ 2
// These three are for the velocity
#define idxUx 3
#define idxUy 4
#define idxUz 5
// Total number of dimensions needed in this problem.
#define VECTOR_SIZE 6

typedef VectorXd (*Rk_Method) (double, VectorXd, double, Rk_Function);

/*
* System of equations:
 * x'' = omega * y' - x' / tau
 * y'' = -omega * x' - y' / tau
 * z'' = - z' / tau
 * This function is for GSL. It's broken up using ux = x', uy = y', and uz = z'
 * to solve 6 first-order ODEs instead of 3 second-order ODEs.
 */
int gsl_particle(double t, const double y[], double f[], void * params) {
  f[idxX] = y[idxUx];
  f[idxY] = y[idxUy];
  f[idxZ] = y[idxUz];
  f[idxUx] = OMEGA * y[idxUy] - TAU_INV * y[idxUx];
  f[idxUy] = - OMEGA * y[idxUx] - TAU_INV * y[idxUy];
  f[idxUz] = - TAU_INV * y[idxUz];
  return GSL_SUCCESS;
}

/*
 * See write-up for why I implemented my own solver. This is the signature
 * my code expects, returning the values of the six first-order ODEs as
 * a VECTOR_SIZE-vector of doubles.
 */
VectorXd particleInField(double t, VectorXd y){
  VectorXd ret(VECTOR_SIZE);
  // Ensure the implementations are identical.
  gsl_particle(t, y.data(), ret.data(), NULL);
  return ret;
}

// FIXME Can I move the GSL functions somewhere else?
// maybe making one GSL runner function that can be used by both problem 1 and 2 -- allowing rk4, rkf45, euler, etc. as the options
// do same thing but with my implementation? mostly there already
// so, idea is having src/GSL.cpp src/solvers.cpp and src/models.cpp or something

/*
 * I'll be honest, to get this solution, I relied on Mathematica solving
 * the system of equations above given the initial conditions of the project.
 * Solving for z(t) is easy, but the others are a little less straightforward.
 * The overall trajectory is a slowing inward spiral in (x, y) with a decaying
 * velocity in z.
 */
VectorXd analytical(double t) {
  VectorXd ret(VECTOR_SIZE);
  ret(idxUx) = 20 * exp(-t * TAU_INV) * cos(t * OMEGA);
  ret(idxUy) = -20 * exp(-t * TAU_INV) * sin(t * OMEGA);
  ret(idxUz) = 2 * exp(-t * TAU_INV);
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

/*
 * Convenience method for writing the configuration options used in the simulation
 * to the output HDF5 file (the DataSet).
 */
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
/*
 * Used for including the timestamp as an attribute in the output.
 */
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

/*
 * Uses a GSL method specified by `methodStr` to integrate the system for
 * `iter` iterations of step size `step` starting at initial conditions
 * (startT, y) where y is a 6-vector, along with particular convergence
 * parameters needed by GSL. Output is a 3 x iter matrix of doubles
 * for the value of (x, y, z) at the end of each iteration
 */
MatrixXd rk_gsl(double y[], int iter, double step, double startT,
                double epsilon_abs, double epsilon_rel, std::string methodStr) {
  double t = startT;
  MatrixXd output(3, iter);

  gsl_odeiv2_system sys = {gsl_particle, NULL, VECTOR_SIZE, NULL};

  const gsl_odeiv2_step_type * method;

  // Select the RK method:
  if (methodStr == "rkf") {
    // Runge-Kutta-Fehlberg
    method = gsl_odeiv2_step_rkf45;
  } else if (methodStr == "rk8pd") {
    // Runge-Kutta Prince-Dormand
    method = gsl_odeiv2_step_rk8pd;
  } else {
    // standard RK4
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

/*
 * Uses a custom method specified by `methodStr` to integrate the system for
 * `iter` iterations of step size `step` starting at initial conditions
 * (startT, state) where state is a 6-vector. Output is a 3 x iter matrix of doubles
 * for the value of (x, y, z) at the end of each iteration.
 */
MatrixXd rk_evan(VectorXd state, int iter, double step, double startT,
                 std::string methodStr) {
   MatrixXd output(3, iter);
   Rk_Method rk;

   if (methodStr == "evan-rk38") {
     // Runge-Kutta with the "3/8" rule
     rk = rk38;
   } else if (methodStr == "evan-rkf") {
     // Runge-Kutta-Fehlberg
     rk = rkf;
   } else {
     // standard RK4
     rk = rk4;
   }

   VectorXd tempState = state;
   for (int i = 0; i < iter; i++) {
     // Take one step at a time.
     tempState = rk(i * step, tempState, step, particleInField);
     output(idxX, i) = tempState(idxX);
     output(idxY, i) = tempState(idxY);
     output(idxZ, i) = tempState(idxZ);
   }
   return output;
}

int run_pr2(INIReader reader) {
  // How many iterations to run
  int iter = reader.GetInteger("problem2", "iter", 10000);
  if (iter <= 0) {
    std::cout << "Should have a positive number of iterations." << std::endl;
    return 1;
  }
  // Step size in time
  double step = reader.GetReal("problem2", "step", 0.01);
  if (step <= 0) {
    std::cout << "Should have a positive step size." << std::endl;
    return 1;
  }

  // Which method to use -- see above functions or input.dat
  string methodStr = reader.Get("problem2", "method", "evan-rk4");

  // Whether to include the analytical solution
  bool verification = reader.GetBoolean("problem2", "verification", "false");

  // GSL parameters
  double epsilon_abs = reader.GetReal("problem2", "epsilon_abs", 1e-6);
  double epsilon_rel = reader.GetReal("problem2", "epsilon_rel", 0.0);

  // Output file for the program -- HDF5 format
  bool trajectory = reader.GetBoolean("problem2", "trajectory", true);
  string outputfile = reader.Get("problem2", "outputfile", "file.h5");


#ifdef DEBUG
  std::cout << "iter:" << iter << "\nstep:" << step <<
               "\nmethod:" << methodStr <<
               "\nverification:" << verification <<
               "\nepsilon_abs:" << epsilon_abs <<
               "\nepsilon_rel:" << epsilon_rel <<
               "\noutputfile:" << outputfile << std::endl;
#endif
  std::string runTime = getTime();

  VectorXd state(VECTOR_SIZE);
  state(idxX) = X0;
  state(idxY) = Y0;
  state(idxZ) = Z0;

  state(idxUx) = UX0;
  state(idxUy) = UY0;
  state(idxUz) = UZ0;

  MatrixXd data(3, iter);

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
  // Ensure that we actually wrote something.
  std::cout << "data(0,0): "<< data(0,0) << std::endl;
#endif

  if (trajectory) {
    H5File * file = new H5File(outputfile, H5F_ACC_TRUNC);

    hsize_t dataset_dims[2] = {3, iter};

    // Allow for chunking and compressing
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

    // Convert from Eigen to a double matrix -- HDF5 doesn't have great
    // support here for Eigen, so have to do it a little by hand.
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
        // Find the value at the end of the step
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
  }

  std::cout << "[pr2_results]" << '\n';
  std::cout << "step: " << step << '\n';
  std::cout << "iter: " << iter << '\n';
  std::cout << "method: " << methodStr << '\n';
  if (verification) {
    VectorXd ana = analytical(step * iter);
    VectorXd result = data.col(iter - 1);
    std::cout << "error: " << (ana.head<3>() - result).norm() << '\n';
    std::cout << "time: ";
  }

  return 0;
}
