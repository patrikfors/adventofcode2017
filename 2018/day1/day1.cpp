#include <fstream>
#include <iostream>
#include <string>
#include <vector>

uint32_t apply_frequency_changes() {
  int32_t acc = 0;
  std::string line;

  while (std::getline(std::cin, line)) {
    int32_t change = std::stoi(line);
    acc += change;
  }

  return acc;
}

int32_t find_recurring_frequency(std::string filename) {
  int32_t acc = 0;
  std::string line;
  size_t const n = 100000;
  uint8_t neg[n];
  uint8_t pos[n];

  for (size_t i = 0; i < n; i++) {
    pos[i] = 0;
    neg[i] = 0;
  }

  pos[0] = 1;

  while (1) {
    std::ifstream input(filename);

    while (std::getline(input, line)) {
      int32_t change = std::stoi(line);
      acc += change;

      if (acc >= 0) {
        if (pos[acc]) {
          return acc;
        }

        pos[acc] = 1;
      } else {
        if (neg[-acc]) {
          return acc;
        }

        neg[-acc] = 1;
      }
    }
  }

  return 0;
}

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  bool arg_fail = false;

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    arg_fail = true;
  } else if (args[1] == "2" && args.size() < 3) {
    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string filename = args[2];

  if (args[1] == "1") {
    std::cout << "resulting frequency: " << apply_frequency_changes()
              << std::endl;
  } else if (args[1] == "2") {

    std::cout << "first duplicate frequency: "
              << find_recurring_frequency(filename) << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
