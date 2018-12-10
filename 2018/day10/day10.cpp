#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <vector>

bool verbose = true;

std::vector<std::string> read_input() {
  std::vector<std::string> rval;

  std::string line;
  uint32_t lines_read = 0;
  while (std::getline(std::cin, line)) {
    rval.emplace_back(line);
    lines_read++;
  }

  if(verbose) {
    std::cout << "Read " << lines_read << " lines from stdin." << std::endl;
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
  std::vector<data_t> rval;
  // position=<-51793, -20673> velocity=< 5,  2>
  static const std::string pattern_string =
      "^position=<\\s*(-?\\d+),\\s*(-?\\d+)> "
      "velocity=<\\s*(-?\\d+),\\s*(-?\\d+)>";
  static std::regex pattern(pattern_string, std::regex_constants::ECMAScript);
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
    }

    else {
      std::cerr << "Failed to match input string \"" << s
                << "\" with pattern \"" << pattern_string << "\"." << std::endl;
      exit(1);
    }
  }

  return rval;
}

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  bool arg_fail = false;

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (args[1] == "1") {
    std::vector<std::string> input = read_input();
    std::vector<data_t> data = parse_input(input);
    for(data_t d : data) {
      std::cout << "(" << std::setw(6) << d.point.x << ", " << std::setw(6) << d.point.y << "):" << 
      "(" << std::setw(6) << d.v.x << ", " << std::setw(6) << d.v.y << ")." << std::endl;
    }
  } else if (args[1] == "2") {
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
