#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::pair<std::uint32_t, std::uint32_t> count_two_and_three(std::string str) {
  uint8_t f[26];

  for (size_t i = 0; i < 26; i++) {
    f[i] = 0;
  }
  for (char c : str) {
    f[c - 'a']++;
  }

  std::pair<std::uint32_t, std::uint32_t> rval = {0, 0};

  for (uint8_t i : f) {
    if (i == 2) {
      rval.first = 1;
    } else if (i == 3) {
      rval.second = 1;
    }
  }

  return rval;
}

bool diff_by_one(std::string a, std::string b) {
  bool diff = false;

  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] != b[i]) {
      if (diff) {
        return false;
      }

      diff = true;
    }
  }

  return diff;
}

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  bool arg_fail = false;

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    arg_fail = true;
    //  } else if (args[1] == "2" && args.size() < 3) {
    //    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (args[1] == "1") {
    std::string line;
    uint32_t two = 0;
    uint32_t three = 0;
    while (std::getline(std::cin, line)) {
      std::pair<std::uint32_t, std::uint32_t> x = count_two_and_three(line);
      if (x.first > 0 || x.second > 0) {
        std::cout << "\"" << line << "\"" << std::endl;
        std::cout << "two: " << x.first << std::endl;
        std::cout << "three: " << x.second << std::endl;
      }

      two += x.first;
      three += x.second;
    }

    std::cout << two << " of them contain a letter which appears exactly twice"
              << std::endl;
    std::cout << three
              << " of them contain a letter which appears exactly three times"
              << std::endl;
    std::cout << "checksum: " << (two * three) << std::endl;

  } else if (args[1] == "2") {
    std::vector<std::string> strings;
    std::string line;

    while (std::getline(std::cin, line)) {
      strings.emplace_back(line);
    }

    for (size_t a = 0; a < strings.size(); a++) {
      for (size_t b = a + 1; b < strings.size(); b++) {
        if (diff_by_one(strings[a], strings[b])) {
          std::cout << "a: " << strings[a] << std::endl;
          std::cout << "b: " << strings[b] << std::endl;

          std::cout << "diff by one!" << std::endl;
          std::cout << "matching: ";
          for (size_t i = 0; i < strings[a].size(); i++) {
            if (strings[a][i] == strings[b][i]) {
              std::cout << strings[a][i];
            }
          }
          std::cout << std::endl;
        }
      }
    }

    exit(0);
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
