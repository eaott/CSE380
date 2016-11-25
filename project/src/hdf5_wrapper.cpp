#include <iostream>
#include <string>

using std::cout;
using std::endl;
#include "H5Cpp.h"
using namespace H5;

int main(int argc, char const *argv[]) {
  hid_t file_id;
  herr_t status;
  const H5std_string FILE_NAME("file.h5");
  H5File* file = new H5File(FILE_NAME, H5F_ACC_TRUNC);
  file_id = H5Fcreate("file.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  status = H5Fclose(file_id);
  cout << status << endl;
  delete file;
  return 0;
}
