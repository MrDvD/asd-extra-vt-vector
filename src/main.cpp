#include <iostream>
#include "vt/vt.h"

int main() {
  vt::vector<int> h = {1, 2, 3};
  std::cout << h.at(2) << " " << h.at(0) << " " << h[1];
  return 0;
}