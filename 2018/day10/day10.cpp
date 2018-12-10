#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <vector>

#include <fmt/core.h>

bool verbose = true;

std::vector<std::string> read_input(std::istream &in) {
  std::vector<std::string> rval;

  std::string line;
  uint32_t lines_read = 0;
  while (std::getline(in, line)) {
    rval.emplace_back(line);
    lines_read++;
  }

  if (verbose) {
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

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);
  bool arg_fail = false;

  if (args.size() < 3 || (args[1] != "1" && args[1] != "2")) {
    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2] <input file>" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (args[1] == "1") {
    std::ifstream file(args[2], std::ifstream::in);
    std::vector<std::string> input = read_input(file);
    std::vector<data_t> data = parse_input(input);

    if (verbose) {
      for (data_t d : data) {
        fmt::print("({:6},{:6}): ({:2}, {:2}).\n", d.point.x, d.point.y, d.v.x,
                   d.v.y);
      }
    }

    int32_t width = 150;
    int32_t height = 50;

    char view[width][height];
    
    bool once_visible = false;
    bool all_clear = false;

    for (uint32_t second = 1;; second++) {
      static const int32_t scale = 1;
      static const int32_t offset = 120;

      fmt::print("Second {}:\n", second);

      memset(view, 0, width * height * sizeof(char));

      all_clear=true;

      // update data points
      for (size_t i=0;i<data.size();i++) {
        data_t d = data[i];
        d.point.x += d.v.x;
        d.point.y += d.v.y;

        data[i] = d;

        if (d.point.x >= (offset+0) && d.point.x < (offset+scale*width) && d.point.y >= (offset+0) &&
            d.point.y < (offset+scale*height)) {
          view[d.point.x/scale-offset][d.point.y/scale-offset]++;
          all_clear = false;
          once_visible = true;
        }
      }

      if(once_visible && all_clear) {
        fmt::print("No visible points.\n");
//        exit(0);
      }

      if(!all_clear) {
        // print
        for (int32_t h = 0; h < height; h++) {
          for (int32_t w = 0; w < width; w++) {
            if(!view[w][h])
              fmt::print(" ");
            else
              fmt::print("{}", (char) ('a'+view[w][h]));
          }

          fmt::print("\n");
        }
        fmt::print("\n");

        std::string dummy;

        std::cout << "Press Enter to continue.";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  } else if (args[1] == "2") {
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
