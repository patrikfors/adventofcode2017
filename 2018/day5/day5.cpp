#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

size_t remaining_units(std::string org)
{
    bool change = true;

    while (change) {
      std::string updated;
      std::string::value_type acc = 0;

      for (auto const c : org) {
        if (acc && (tolower(acc) == tolower(c)) && (acc != c)) {
          // they exclude eachother
          acc = 0;
        } else {
          if (acc) {
            updated.push_back(acc);
          }

          acc = c;
        }
      }

      if (acc) {
        updated.push_back(acc);
      }

      change = (updated != org);
      org = updated;
    }

  return org.size();
}

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  bool arg_fail = false;

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (args[1] == "1") {
    std::string org;
    std::getline(std::cin, org);

    size_t remaining = remaining_units(org);
    std::cout << "Units remaining: " << remaining << std::endl;
  } else if (args[1] == "2") {
    std::string org;
    std::getline(std::cin, org);

    int smallest = 9999999;
    char smallest_c = 0;

    for(char c = 'a';c <= 'z'; c++) {
      std::string reduced = org;
      reduced.erase(std::remove(reduced.begin(), reduced.end(), std::tolower(c)), reduced.end());
      reduced.erase(std::remove(reduced.begin(), reduced.end(), std::toupper(c)), reduced.end());
      
      size_t remaining = remaining_units(reduced);
      std::cout << c << " remaining = " << remaining << std::endl;
      if(remaining < smallest) {
        smallest = remaining;
        smallest_c = c;
      }
    }

    std::cout << "Least number of units (" << smallest << ") with " << smallest_c << " removed." << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
