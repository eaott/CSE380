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

TEST_CASE( "1 iteration -- GSL vs mine, full problem [evan-gsl]" ) {
  ofstream out;
  out.open("test_config_evan.ini");
  out << "[problem2]\n";
  out << "iter=10000\n"
      << "step=0.0001\n"
      << "method=evan-rk4\n"
      << "verification=true\n"
      << "outputfile=evan.h5";
  out.close();
  INIReader evan("test_config_evan.ini");
  if (evan.ParseError() < 0) {
    FAIL("Can't load test_config_evan.ini");
  }

  out.open("test_config_gsl.ini");
  out << "[problem2]\n";
  out << "iter=1\n"
      << "step=0.0001\n"
      << "method=rk4\n"
      << "verification=false\n"
      << "outputfile=gsl.h5";
  out.close();
  INIReader gsl("test_config_gsl.ini");
  if (gsl.ParseError() < 0) {
    FAIL("Can't load test_config_gsl.ini");
  }

  run_pr2(evan);
  run_pr2(gsl);

  // H5File * evan_file = new H5File("evan.h5", H5F_ACC_TRUNC);
  // H5File * gsl_file = new H5File("gsl.h5", H5F_ACC_TRUNC);
  //
  // DataSet evan_ds = evan_file->openDataSet("dataset");
  // DataSet gsl_ds = gsl_file->openDataSet("dataset");
  //
  // double data_out[3][1];
  // evan_ds.read(data_out, PredType::NATIVE_DOUBLE);
  // std::cout << "OMG:" << data_out[0][0] << std::endl;


  // FIXME read in both H5 files and ensure the results are within
  // numerical tolerance
  return;
}
