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
DEFINE_string(f, "", "Input data");


static bool part_validator(char const *flagname, int32_t value) {
  return (value > 0);
}

DEFINE_validator(part, &part_validator);

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

void print() {
}

void wait_for_enter()
{
  fmt::print(stderr, "Press Enter to quit.\n");
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(int argc, char *argv[]) {
  atexit(wait_for_enter);

  gflags::SetUsageMessage("[-v] [-f <input>  -part <1|2>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // positional arguments
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);

  std::vector<std::string> input;
  if (FLAGS_f.size() > 0) {
    std::ifstream file(FLAGS_f, std::ifstream::in);
    if (FLAGS_v) {
      fmt::print("Reading input from {}.\n", FLAGS_f);
    }
    if (!file) {
      fmt::print("Unable to open input file {}.\n", FLAGS_f);
      exit(1);
    }

    input = read_input(file);
  } else {
    if (FLAGS_v) {
      fmt::print("Reading input from stdin.\n");
    }

    input = read_input(std::cin);
  }

  if (0 != parse_input(input)) {
    fmt::print(stderr, "Failed to parse input!");
    exit(1);
  }

  if (FLAGS_v) {
    fmt::print("Parsing complete.\n");
  }

  if (FLAGS_part == 1) {
  } else if (FLAGS_part == 2) {
  } else {
    fmt::print(stderr, "Invalid part {}.\n", FLAGS_part);
    exit(1);
  }

  exit(0);
}
