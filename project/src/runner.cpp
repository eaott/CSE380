/*
 * Basic runner that decides between loading one of the two problems.
 */
#include <iostream>
#include "INIReader.h"
#include "pr1.hpp"
#include "pr2.hpp"

int main(int argc, char const *argv[]) {
  std::string filename = "input.dat";
#ifdef DEBUG
  std::cout << argc << '\n';
#endif
  if (argc > 1) {
    filename = argv[1];
#ifdef DEBUG
    std::cout << filename << '\n';
#endif
  }
  INIReader reader(filename);
  if (reader.ParseError() < 0) {
    std::cout << "Can't load " << filename << "\n" << std::endl;
    return 1;
  }
  string methodStr = reader.Get("runner", "problem", "problem1");
  if (methodStr == "problem2") {
    run_pr2(reader);
  } else {
    run_pr1(reader);
  }
  return 0;
}
