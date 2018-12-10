#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <vector>

#include <fmt/core.h>
#include <gflags/gflags.h>

DEFINE_bool(v, false, "Be verbose");
DEFINE_int32(part, 1, "Problem part");

static bool part_validator(char const *flagname, int32_t value)
{
  return (value == 1 || value == 2);
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

struct data_t {
  struct {
    int32_t x;
    int32_t y;
  } point;

  struct {
    int32_t x;
    int32_t y;
  } v;

  data_t(int32_t px, int32_t py, int32_t vx, int32_t vy) {
    this->point.x = px;
    this->point.y = py;
    this->v.x = vx;
    this->v.y = vy;
  }
};

std::vector<data_t> parse_input(std::vector<std::string> const &input) {
  static const std::string pattern_string =
      "^position=<\\s*(-?\\d+),\\s*(-?\\d+)> "
      "velocity=<\\s*(-?\\d+),\\s*(-?\\d+)>";
  static std::regex pattern(pattern_string, std::regex_constants::ECMAScript);

  std::vector<data_t> rval;

  for (auto s : input) {
    std::smatch matches;
    if (std::regex_search(s, matches, pattern)) {
      int32_t x;
      int32_t y;
      int32_t vx;
      int32_t vy;
      x = std::stoi(matches[1].str());
      y = std::stoi(matches[2].str());
      vx = std::stoi(matches[3].str());
      vy = std::stoi(matches[4].str());
      rval.emplace_back(data_t(x, y, vx, vy));
    } else {
      fmt::print(stderr,
                 "Failed to match input string \"{}\" with pattern \"{}\".\n",
                 s, pattern_string);
      exit(1);
    }
  }

  return rval;
}

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("[-v] -part <1|2> <input file>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // positional arguments
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);
  bool arg_fail = false;

  if(args.size() < 2) {
    std::cout << "Usage: " << gflags::ProgramInvocationShortName() << " " << gflags::ProgramUsage() << std::endl;
    exit(1);
  }

  if (FLAGS_part == 1) {
    std::ifstream file(args[1], std::ifstream::in);
    std::vector<std::string> input = read_input(file);
    std::vector<data_t> data = parse_input(input);

    if (FLAGS_v) {
      for (data_t d : data) {
        fmt::print("({:6},{:6}): ({:2}, {:2}).\n", d.point.x, d.point.y, d.v.x,
                   d.v.y);
      }
    }

    int32_t width = 150;
    int32_t height = 50;

    char view[width][height];

    for (uint32_t second = 1;; second++) {
      memset(view, 0, width * height * sizeof(char));

      int32_t x_offset = INT32_MAX;
      int32_t y_offset = INT32_MAX;

      // update data points
      for (size_t i = 0; i < data.size(); i++) {
        data_t d = data[i];
        d.point.x += d.v.x;
        d.point.y += d.v.y;

        data[i] = d;
      }

      bool is_aligned = true;
      // aligned is defined as every point next to another..
      for (size_t fst = 0; fst < data.size(); fst++) {
        bool neighbour_found = false;

        for (size_t snd = 0; snd < data.size(); snd++) {
          if(snd==fst) {
            continue;
          }

          int32_t x_dist = abs(data[fst].point.x - data[snd].point.x);
          int32_t y_dist = abs(data[fst].point.y - data[snd].point.y);

          if (x_dist <= 1 && y_dist <= 1) {
            neighbour_found = true;
          }
        }

        if (!neighbour_found) {
          is_aligned = false;
          break;
        }

        if (abs(data[fst].point.x) < x_offset) {
          x_offset = data[fst].point.x;
        }

        if (abs(data[fst].point.y) < y_offset) {
          y_offset = data[fst].point.y;
        }
      }

      if (is_aligned) {
        fmt::print("Second {}:\n", second);
        x_offset = -x_offset;
        y_offset = -y_offset;

        memset(view, 0, width * height * sizeof(char));
        for (size_t fst = 0; fst < data.size(); fst++) {
          int32_t x = data[fst].point.x + x_offset;
          int32_t y = data[fst].point.y + y_offset;

          if (x >= 0 && x < width && y >= 0 && y < height) {
            view[x][y] = '#';
          }
        }

        // print
        for (int32_t h = 0; h < height; h++) {
          for (int32_t w = 0; w < width; w++) {
            if (!view[w][h])
              fmt::print(" ");
            else {
              fmt::print("{}", view[w][h]);
            }
          }

          fmt::print("\n");
        }
        fmt::print("\n");

        std::cout << "Press Enter to continue." << std::flush; 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        exit(0);
      }
    }
  } else if (FLAGS_part == 2) {
    std::cout << "not implemented yet" << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
