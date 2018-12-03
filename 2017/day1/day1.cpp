#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

uint32_t sum_halfway_digits(std::string input) {
  uint32_t acc = 0;

  if (input.size() < 2) {
    std::cerr << "len < 2" << std::endl;
    return 0;
  }

  if (input.size() % 2 != 0) {
    std::cerr << "len < 2" << std::endl;
    return 0;
  }

  size_t offset = input.size() / 2;

  for (auto const &c : input) {
    if (c < '0' || c > '9') {
      std::cerr << "not digit" << std::endl;
      return 0;
    }
  }

  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] == input[(i + offset) % input.size()]) {
      acc += input[i] - '0';
    }
  }

  return acc;
}

uint32_t sum_matching_digits(std::string input) {
  uint32_t acc = 0;

  if (input.size() < 2) {
      std::cerr << "len < 2" << std::endl;
    return 0;
  }

  for (auto const &c : input) {
    if (c < '0' || c > '9') {
        std::cerr << "not digit" << std::endl;
      return 0;
    }
  }

  for (size_t i = 1; i <= input.size(); i++) {
    if (input[i % input.size()] == input[i - 1]) {
      acc += input[i - 1] - '0';
    }
  }

  return acc;
}

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;

  while (std::getline(std::cin, line)) {
    if (args[1] == "1") {
      std::cout << sum_matching_digits(line) << std::endl;
    } else if (args[1] == "2") {
      std::cout << sum_halfway_digits(line) << std::endl;
    }
  }
}
