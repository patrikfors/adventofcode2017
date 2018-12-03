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

uint32_t count_squares(std::string key)
{
  for(int i=0;i<128;i++) {
    std::stringstream hash;
    hash << key << "-" << i;

    printf("%s\r\n", hash.str().c_str());
  }

  return 0;
}

int main(int argc, char const* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::string line;
  std::string last_line;
  bool part2 = false;

  if (args.size() >= 2 && args[1] == "2") {
    part2 = true;
  }


  while (std::getline(std::cin, line)) {
    last_line = line;

    printf("%d squares are used.\r\n", count_squares(line));
  }
}
