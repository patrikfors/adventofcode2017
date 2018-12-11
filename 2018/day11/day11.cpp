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
DEFINE_int32(serial, 0, "Serial No");

static bool part_validator(char const *flagname, int32_t value) {
  return (value == 1 || value == 2);
}

static bool serial_validator(char const *flagname, int32_t value) {
  return (value > 0);
}

DEFINE_validator(part, &part_validator);
DEFINE_validator(serial, &serial_validator);

int32_t power_level(int32_t serial_no, int32_t x, int32_t y) {
  int32_t rack_id =
      x +
      10; // Find the fuel cell's rack ID, which is its X coordinate plus 10.
  int32_t power_level =
      rack_id *
      y; // Begin with a power level of the rack ID times the Y coordinate.
  power_level += serial_no; // Increase the power level by the value of the grid
                            // serial number (your puzzle input).
  power_level *=
      rack_id; // Set the power level to itself multiplied by the rack ID.

  // Keep only the hundreds digit of the power level (so 12345 becomes 3;
  // numbers with no hundreds digit become 0).
  if (power_level < 100) {
    power_level = 0;
  } else {
    power_level %= 1000; //  921234 -> 234
    power_level = power_level / 100;
  }

  power_level -= 5; // Subtract 5 from the power level.

  return power_level;
}

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

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("[-v] -part <1|2> <input file>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // positional arguments
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);

  if (FLAGS_v) {
    /*
          fmt::print("power_level(8, 3, 5) = {}\n", power_level(8, 3, 5));
          fmt::print("power_level(57, 122, 79) = {}\n", power_level(57, 122,
    79));
          fmt::print("power_level(39, 217, 196) = {}\n", power_level(39, 217,
    196));
          fmt::print("power_level(71, 101, 153) = {}\n", power_level(71, 101,
    153));
    fmt::print("Fuel cell at  122,79, grid serial number 57: power level
    -5.\n");
    fmt::print("Fuel cell at 217,196, grid serial number 39: power level 0.\n");
    fmt::print("Fuel cell at 101,153, grid serial number 71: power level 4.\n");
    */
  }

  int32_t max_x;
  int32_t max_y;
  int32_t max_sum = INT32_MIN;
  int32_t max_size = 0;

  int32_t size_min = 1;
  int32_t size_max = 300;

  if (FLAGS_part == 1) {
    size_min = 3;
    size_max = 3;
  }

  for (int32_t x = 1; x <= 300; x++) {
    for (int32_t y = 1; y <= 300; y++) {
      for (int32_t size = size_min; size <= size_max; size++) {
        if (x + size - 1 > 300) {
          continue;
        }
        if (y + size - 1 > 300) {
          continue;
        }

        int32_t sum = 0;

        for (int32_t xs = 0; xs < size; xs++) {
          for (int32_t ys = 0; ys < size; ys++) {
            sum += power_level(FLAGS_serial, x + xs, y + ys);
          }
        }

        if (sum > max_sum) {
          max_sum = sum;
          max_x = x;
          max_y = y;
          max_size = size;
        }
      }
    }
  }
  fmt::print("Max sum, {}, found at ({}, {}) with size {}.\n", max_sum, max_x,
             max_y, max_size);

  exit(0);
}
