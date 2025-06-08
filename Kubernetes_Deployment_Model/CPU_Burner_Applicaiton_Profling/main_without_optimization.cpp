#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <thread>

uint64_t inefficient_calc_sum() {
  uint64_t result = 0;
  for (int i = 1; i <= 1'000'000'000; ++i) {
    result += i;
  }
  return result;
}

void shuffle_some_array() {
  std::vector<int> vec;
  for (int i = 0; i < 3'000'000; ++i) {
    vec.push_back(std::rand());
  }
  for (int i = 0; i < 20; ++i) {
    std::shuffle(vec.begin(), vec.end(), std::mt19937());
  }
}

void my_function() {
  shuffle_some_array();
  std::cout << inefficient_calc_sum() << std::endl;
}

int main() {
  int i = 0;
  while (true) {  // Infinite loop
    std::cout << "Iteration:" << i << " ";
    my_function();
    std::this_thread::sleep_for(std::chrono::seconds(5));  // 10-second sleep
    i++;
  }
  return 0;
}
