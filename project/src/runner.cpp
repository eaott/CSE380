#include <iostream>
#include "INIReader.h"
#include "pr1.hpp"
#include "pr2.hpp"

int main(int argc, char const *argv[]) {
  INIReader reader("test.ini");
  if (reader.ParseError() < 0) {
    std::cout << "Can't load test.ini\n" << std::endl;
    return 1;
  }
  string methodStr = reader.Get("runner", "problem", "problem1");
  if (methodStr == "problem2") {
    run_pr2("test.ini");
  } else {
    run_pr1("test.ini");
  }
  return 0;
}
