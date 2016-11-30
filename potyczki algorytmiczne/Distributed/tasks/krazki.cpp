#include "krazki.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <mutex>

namespace {

int pipe_height, number_of_discs;
std::vector<long long int> pipe, discs;

void Init() {
  static bool initialized = false;
  if (!initialized) {
    initialized = true;
    std::cin >> pipe_height >> number_of_discs;
    pipe.resize(pipe_height);
    for (int i = 0; i < pipe_height; i++) {
      std::cin >> pipe[i];
    }
    discs.resize(number_of_discs);
    for (int i = 0; i < number_of_discs; i++) {
      std::cin >> discs[i];
    }
  }
}

int force_init = [](){ return Init(), 1;}();

}  // namespace

int PipeHeight() {
  Init();
  return pipe_height;
}

int NumberOfDiscs() {
  Init();
  return number_of_discs;
}

long long int HoleDiameter(long long int i) {
  Init();
  assert(1 <= i && i <= pipe_height);
  return pipe[i - 1];
}

long long int DiscDiameter(long long int j) {
  Init();
  assert(1 <= j && j <= number_of_discs);
  return discs[j - 1];
}
