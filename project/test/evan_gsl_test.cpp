#include <iostream>
#include <fstream>
#include "rk.hpp"
#include "INIReader.h"
using std::cout;
using std::endl;
#include "H5Cpp.h"
#include "pr2.hpp"

#include "catch.hpp"
#define USE_MATH_DEFINES
#include <cmath>
#include "utils.hpp"
#include "rk_utils.hpp"


using namespace Eigen;
using namespace std;
using namespace H5;

TEST_CASE( "1 iteration -- GSL vs mine, with HDF5 [evan-gsl]" ) {
  ofstream out;
  out.open("test_config_evan.ini");
  out << "[problem2]\n";
  out << "iter=1\n"
      << "step=0.001\n"
      << "method=evan-rk4\n"
      << "trajectory=true\n"
      << "verification=false\n"
      << "outputfile=evan.h5";
  out.close();
  INIReader evan("test_config_evan.ini");
  if (evan.ParseError() < 0) {
    FAIL("Can't load test_config_evan.ini");
  }

  out.open("test_config_gsl.ini");
  out << "[problem2]\n";
  out << "iter=1\n"
      << "step=0.001\n"
      << "method=rk4\n"
      << "trajectory=true\n"
      << "verification=false\n"
      << "outputfile=gsl.h5";
  out.close();
  INIReader gsl("test_config_gsl.ini");
  if (gsl.ParseError() < 0) {
    FAIL("Can't load test_config_gsl.ini");
  }

  run_pr2(evan);
  run_pr2(gsl);
  H5File * evan_file = new H5File("evan.h5", H5F_ACC_RDONLY);
  H5File * gsl_file = new H5File("gsl.h5", H5F_ACC_RDONLY);

  DataSet evan_ds = evan_file->openDataSet("dataset");
  DataSet gsl_ds = gsl_file->openDataSet("dataset");

  double evan_data[3][1];
  evan_ds.read(evan_data, PredType::NATIVE_DOUBLE);

  double gsl_data[3][1];
  gsl_ds.read(gsl_data, PredType::NATIVE_DOUBLE);

#ifdef DEBUG
  std::cout << "GSL: (" <<
               gsl_data[0][0] << ", " <<
               gsl_data[1][0] << ", " <<
               gsl_data[2][0] << ") " <<
               "Evan: (" <<
               evan_data[0][0] << ", " <<
               evan_data[1][0] << ", " <<
               evan_data[2][0] << ")\n";
#endif
  REQUIRE(equal(gsl_data[0][0], evan_data[0][0]));
  REQUIRE(equal(gsl_data[1][0], evan_data[1][0]));
  REQUIRE(equal(gsl_data[2][0], evan_data[2][0]));

  delete evan_file;
  delete gsl_file;
  return;
}
