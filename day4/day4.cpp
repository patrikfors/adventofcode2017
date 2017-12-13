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

#include "absl/strings/str_split.h"

std::string sort_word(std::string const& input) {
  std::stringstream output;

  std::list<char> lst;
  for(auto const& c : input) {
    lst.emplace_back(c);
  }

  lst.sort();

  for(auto const& c : lst) {
    output << c;
  }

  return output.str();  
}

int main(int argc, char const* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::string line;

  uint32_t valid = 0;
  uint32_t invalid = 0;
  std::string last_line;
  bool use_anagram = false;

  if (args.size() >= 2 && args[1] == "2") {
    use_anagram = true;
  }

  while (std::getline(std::cin, line)) {
    last_line = line;
    std::vector<std::string> input_words = absl::StrSplit(line, " ");
    std::set<std::string> words;

    bool is_valid = true;
    for (auto const& w : input_words) {
      std::string tw;
      if (use_anagram) {
        tw = sort_word(w);
      }
      else {
        tw = w;
      }

      if (words.find(tw) != words.end()) {
        printf("Invalid passphrase, word %s already exists.\r\n", tw.c_str());
        is_valid = false;
        break;
      }
      words.emplace(tw);
    }

    if (is_valid) {
      valid++;
    } else {
      invalid++;
    }
  }

  printf("Total number of passphrases: %d\r\n", invalid + valid);
  printf("Valid: %d\r\n", valid);
  printf("Invalid: %d\r\n", invalid);
  printf("last line: %s\r\n", last_line.c_str());
}
