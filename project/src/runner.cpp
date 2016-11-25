#include <iostream>
#include "INIReader.h"

// FIXME: convert the other problems to run using non-main method.
// will need to create .hpp files for each

int main(int argc, char const *argv[]) {
  INIReader reader("test.ini");
  if (reader.ParseError() < 0) {
    std::cout << "Can't load test.ini\n" << std::endl;
    return 1;
  }
  string methodStr = reader.Get("runner", "problem", "problem1");
  if (methodStr == "problem2") {

  } else {

  }
  return 0;
}
