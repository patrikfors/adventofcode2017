#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"


int main(int argc, char const* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::string line;
  std::string last_line;
  bool even_stranger = false;

  if (args.size() >= 2 && args[1] == "2") {
    even_stranger = true;
  }
   
  std::vector<int> instructions;

  while (std::getline(std::cin, line)) {
    last_line = line;

    int instruction;
    if (!absl::SimpleAtoi(line, &instruction)) {
      std::cerr << "Failed to convert '" << line << "' to a number." << std::endl;
      exit(EXIT_FAILURE);
    }

    instructions.emplace_back(instruction);
  }

  printf("Total number of instructions: %zd\r\n", instructions.size());
  printf("last line: %s\r\n", last_line.c_str());

  int ip = 0;
  uint32_t steps = 0;
  while (ip >=0 && ip < instructions.size()) 
  {
    int next_ip = ip + instructions[ip];
    if (even_stranger && (instructions[ip] >= 3)) {
      instructions[ip] -= 1;  
    }
    else {
      instructions[ip] += 1;
    }
    

    ip = next_ip;
    steps++;
  }

  printf("Finished in %d steps.\r\n", steps);
}
