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

static bool part_validator(char const *flagname, int32_t value)
{
  return (value == 1 || value == 2);
}

DEFINE_validator(part, &part_validator);

int32_t power_level(int32_t serial_no, int32_t x, int32_t y)
{
/*
For example, to find the power level of the fuel cell at 3,5 in a grid with serial number 8:

The rack ID is 3 + 10 = 13.
The power level starts at 13 * 5 = 65.
Adding the serial number produces 65 + 8 = 73.
Multiplying by the rack ID produces 73 * 13 = 949.
The hundreds digit of 949 is 9.
Subtracting 5 produces 9 - 5 = 4.
*/

  int32_t rack_id = x+10; // Find the fuel cell's rack ID, which is its X coordinate plus 10.
if(FLAGS_v) fmt::print("rack_id {}\n", rack_id);
  int32_t power_level = rack_id * y; // Begin with a power level of the rack ID times the Y coordinate.
if(FLAGS_v) fmt::print("power_level={}\n", power_level);
  power_level += serial_no; // Increase the power level by the value of the grid serial number (your puzzle input).
if(FLAGS_v) fmt::print("power_level={}\n", power_level);
  power_level *= rack_id; // Set the power level to itself multiplied by the rack ID.
if(FLAGS_v) fmt::print("power_level={}\n", power_level);

  // Keep only the hundreds digit of the power level (so 12345 becomes 3; numbers with no hundreds digit become 0).
  if(power_level < 100) {
    power_level = 0;
  }
  else {
    power_level %= 1000; //  921234 -> 234
if(FLAGS_v) fmt::print("power_level={}\n", power_level);
    power_level = power_level / 100;
if(FLAGS_v) fmt::print("power_level={}\n", power_level);
  }

  power_level -= 5; // Subtract 5 from the power level.
if(FLAGS_v) fmt::print("power_level={}\n", power_level);

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

  if (FLAGS_part == 1) {
    if (FLAGS_v) {
      fmt::print("power_level(8, 3, 5) = {}\n", power_level(8, 3, 5));
      fmt::print("power_level(57, 122, 79) = {}\n", power_level(57, 122, 79));
      fmt::print("power_level(39, 217, 196) = {}\n", power_level(39, 217, 196));
      fmt::print("power_level(71, 101, 153) = {}\n", power_level(71, 101, 153));
fmt::print("Fuel cell at  122,79, grid serial number 57: power level -5.\n");
fmt::print("Fuel cell at 217,196, grid serial number 39: power level  0.\n");
fmt::print("Fuel cell at 101,153, grid serial number 71: power level  4.\n");
    }
  } else if (FLAGS_part == 2) {
    std::cout << "not implemented yet" << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
