#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"

uint32_t get_max_difference(std::string input) {
  std::vector<absl::string_view> v = absl::StrSplit(input, absl::ByAnyChar(" \t"), absl::SkipEmpty());

  uint32_t lowest = UINT32_MAX;
  uint32_t highest = 0;
  for (auto const& s : v) {
    uint32_t i;

    if (!absl::SimpleAtoi(s, &i)) {
      std::cerr << "Not a number: " << s << std::endl;
      return 0;
    }

    lowest = std::min<uint32_t>(lowest, i);
    highest = std::max<uint32_t>(highest, i);
  }

  return highest - lowest;
}

uint32_t get_evenly_division(std::string input) {
  std::vector<absl::string_view> strings = absl::StrSplit(input, absl::ByAnyChar(" \t"), absl::SkipEmpty());
  std::vector<unsigned> ints;

  for (auto const& s : strings) {
    uint32_t i;

    if (!absl::SimpleAtoi(s, &i)) {
      std::cerr << "Not a number: " << s << std::endl;
      return 0;
    }

    auto it = std::upper_bound(ints.begin(), ints.end(), i);
    ints.insert(it, i);

    for (int i = ints.size() - 1; i >= 0; i--) {
      for (int j = i - 1; j >= 0; j--) {
        if ((ints[i] % ints[j]) == 0) {
          return ints[i] / ints[j];
        }
      }
    }
  }

  return 0;
}

int main(int argc, char const* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;

  uint32_t sum = 0;
  while (std::getline(std::cin, line)) {
    if (args[1] == "2") {
      sum += get_evenly_division(line);
      continue;
    }

    sum += get_max_difference(line);
  }

  std::cout << "Sum: " << sum << std::endl;
}
