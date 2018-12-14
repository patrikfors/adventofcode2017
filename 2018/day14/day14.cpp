#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <set>
#include <vector>

#include <fmt/core.h>
#include <gflags/gflags.h>

DEFINE_bool(v, false, "Be verbose");
DEFINE_int32(part, 0, "Problem part");
DEFINE_int32(input, 0, "Puzzle input");


static bool part_validator(char const *flagname, int32_t value) {
  return (value > 0);
}

DEFINE_validator(part, &part_validator);
DEFINE_validator(input, &part_validator);

std::vector<std::string> read_input(std::istream &in) {
  std::vector<std::string> rval;

  std::string line;
  uint32_t lines_read = 0;
  while (std::getline(in, line)) {
    rval.emplace_back(line);
    lines_read++;
  }

  if (FLAGS_v) {
    fmt::print("Read {} lines.\n", lines_read);
  }

  return rval;
}

int parse_input(std::vector<std::string> input) {

  return 0;
}

void print(std::vector<uint8_t> recipes, size_t fst, size_t snd) {
  for (size_t i = 0; i < recipes.size(); i++) {
    if (i == fst)
      fmt::print("({})", recipes[i]);
    else if (i == snd)
      fmt::print("[{}]", recipes[i]);
    else
      fmt::print(" {} ", recipes[i]);
  }

  fmt::print("\n");
}

inline static void append_sum_of_digits(std::vector<uint8_t> &v, uint8_t sum) {
  if (sum == 0) { v.emplace_back(0); }
  if (sum == 1) { v.emplace_back(1); }
  if (sum == 2) { v.emplace_back(2); }
  if (sum == 3) { v.emplace_back(3); }
  if (sum == 4) { v.emplace_back(4); }
  if (sum == 5) { v.emplace_back(5); }
  if (sum == 6) { v.emplace_back(6); }
  if (sum == 7) { v.emplace_back(7); }
  if (sum == 8) { v.emplace_back(8); }
  if (sum == 9) { v.emplace_back(9); }
  if (sum == 10) { v.emplace_back(1); v.emplace_back(0); }
  if (sum == 11) { v.emplace_back(1); v.emplace_back(1); }
  if (sum == 12) { v.emplace_back(1); v.emplace_back(2); }
  if (sum == 13) { v.emplace_back(1); v.emplace_back(3); }
  if (sum == 14) { v.emplace_back(1); v.emplace_back(4); }
  if (sum == 15) { v.emplace_back(1); v.emplace_back(5); }
  if (sum == 16) { v.emplace_back(1); v.emplace_back(6); }
  if (sum == 17) { v.emplace_back(1); v.emplace_back(7); }
  if (sum == 18) { v.emplace_back(1); v.emplace_back(8); }
}

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("[-v] -part <1|2>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // positional arguments
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);

  //uint32_t puzzle_input = 147061;
  //uint32_t puzzle_input = 9;
  //uint32_t puzzle_input = 2018;
  size_t puzzle_input = FLAGS_input;

  if (FLAGS_v) {
    fmt::print("Parsing complete.\n");
  }

  if (FLAGS_part == 3) {
    uint8_t sum_of_digits[18] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8 };

    fmt::print("inline static void append_sum_of_digits(std::vector<uint8_t> &v, uint8_t sum) {{\n");

    for (int i = 0; i < 18; i++) {
      fmt::print("if (sum=={}) {{", i);

      if (i >= 10) {
        fmt::print("v.emplace_back(1); v.emplace_back({}); }}\n", i % 10);
      }
      else {
        fmt::print("v.emplace_back({}); }}\n", i % 10);
      }
    }

    fmt::print("}}\n");
  }

  std::vector<uint8_t> recipes;
  recipes.emplace_back(3);
  recipes.emplace_back(7);
  size_t fst = 0;
  size_t snd = 1;

  if (FLAGS_v) {
    print(recipes, fst, snd);
  }
  
  while(recipes.size() < (puzzle_input+10)) {
    append_sum_of_digits(recipes, recipes[fst] + recipes[snd]);
    fst = (fst + (recipes[fst] + 1)) % recipes.size();
    snd = (snd + (recipes[snd] + 1)) % recipes.size();

    if (FLAGS_v) {
      print(recipes, fst, snd);
    }
  }

  if (FLAGS_part == 1) {
    fmt::print("Result = ");
    /*for (std::vector<uint8_t>::iterator i = recipes.begin() + puzzle_input; i != recipes.end(); i++) {
      fmt::print("{}", (int)*i);
    }*/

    
    //for (int i = 0; i < recipes.size(); i++) {
    for (int i = puzzle_input; i < puzzle_input + 10; i++) {
      fmt::print("{}", (int) recipes[i]);
    }
    
    fmt::print("\n");

  } else if (FLAGS_part == 2) {
  } else {
    fmt::print(stderr, "Invalid part {}.\n", FLAGS_part);
  }

  std::cout << "Press Enter to quit." << std::flush;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  exit(0);
}
