#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
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

std::string get_initial_state(std::vector<std::string> const& data)
{
  static const std::string pattern_string = "^initial state: ([#\\.]+)";
  static std::regex pattern(pattern_string, std::regex_constants::ECMAScript);

  for (auto s : data) {
    std::smatch matches;
    if (std::regex_search(s, matches, pattern)) {
      return matches[1].str();
    }
  }

  return "FAILED!";
}

int get_plant_mask(std::vector<std::string> const& data, bool mask[32])
{
  static const std::string pattern_string = "^([#\\.]+) => #";
  static std::regex pattern(pattern_string, std::regex_constants::ECMAScript);

  for(size_t i=0;i<32;i++) {
    mask[i] = false;
  }

  for (auto line : data) {
    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
      std::string bits_str = matches[1].str();
      size_t bits = bits_str.size();
      uint8_t idx = 0;

      for(size_t i = 0;i<bits;i++) {
        if (bits_str[i] == '#') {
          idx |= (1 << (bits - i - 1));
        }
      }

      mask[idx] = true;

      if (FLAGS_v) {
        fmt::print("bit string {} => bits = {}.\n", bits_str, idx);
      }
    }
    else if (FLAGS_v) {
      fmt::print("skipping input \"{}\".\n", line);
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("[-v] [-f <input>  -part <1|2>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // positional arguments
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);

  std::vector<std::string> input;
  if (FLAGS_f.size() > 0) {
    std::ifstream file(args[1], std::ifstream::in);
    input = read_input(file);
  }
  else {
    input = read_input(std::cin);
  }

  if (FLAGS_v) {
  }

  std::string state;
  bool mask[(1 << 5)];

  state = get_initial_state(input);
  get_plant_mask(input, mask);

  if (FLAGS_v) {
    fmt::print("Initial state: {}\n", state);

    for(size_t i=0;i<32;i++) {
      fmt::print("mask[{}] = {}.\n", i, mask[i]);
    }
  }


  if (FLAGS_part == 1) {
  }
  else {
    fmt::print(stderr, "Invalid part {}.\n", FLAGS_part);
  }

  exit(0);
}
