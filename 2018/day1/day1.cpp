#include <iostream>
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

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;

  if (args[1] == "1") {
    std::cout << "resulting frequencey: " << apply_frequency_changes()
              << std::endl;
  } else if (args[1] == "2") {
    std::cout << "not implemented yet" << std::endl;
    exit(2);
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
