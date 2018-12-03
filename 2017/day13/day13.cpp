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

std::vector<int> move_scanners(std::vector<int> const& layers, std::vector<int> const& scanners, std::vector<int>* scanner_directions) {
  if (layers.size() != scanners.size()) {
    fprintf(stderr, "Layers should have same size as scanners (%zd vs %zd).\r\n", layers.size(), scanners.size());
    exit(EXIT_FAILURE);
  }

  std::vector<int> rval(layers.size());

  for (size_t i = 0; i < layers.size(); i++) {
    if (!layers[i]) {
      continue;
    }

    int new_pos = scanners[i] + (*scanner_directions)[i];
    if (new_pos < 0 || new_pos >= layers[i]) {
      (*scanner_directions)[i] = -(*scanner_directions)[i];
      new_pos = scanners[i] + (*scanner_directions)[i];
    }

    if (new_pos < 0 || new_pos >= layers[i]) {
      fprintf(stderr, "WTF!");
      exit(EXIT_FAILURE);
    }

    rval[i] = new_pos;
  }

  return rval;
}

uint32_t find_severity(std::vector<int> const& layers, uint32_t delay, bool* caught = nullptr) {
  std::vector<int> scanners(layers.size(), 0);
  std::vector<int> scanner_directions(layers.size(), 1);

  uint32_t acc_severity = 0;
  int my_pos = -1;

  for (uint32_t i = 0; i < delay; i++) {
    scanners = move_scanners(layers, scanners, &scanner_directions);
  }

  for (size_t i = 0; i < layers.size(); i++) {
    uint32_t severity = 0;
    // move into pos
    my_pos++;

    if (layers[i] && scanners[i] == 0) {
      //      printf("caught at layer %zd\r\n", i);
      severity = i * layers[i];
      if (caught) {
        *caught = true;
      }
    }

    acc_severity += severity;

    // move scanners
    scanners = move_scanners(layers, scanners, &scanner_directions);
  }

  return acc_severity;
}

bool find_severity2(std::vector<int> const& layers, std::vector<int> scanners, std::vector<int> scanner_directions) {
  int my_pos = -1;

  for (size_t i = 0; i < layers.size(); i++) {
    // move into pos
    my_pos++;

    if (layers[i] && scanners[i] == 0) {
//      printf("caught at layer %zd\r\n", i);
      return true;
    }

    // move scanners
    scanners = move_scanners(layers, scanners, &scanner_directions);
  }

  //printf("not caught\r\n");
  return false;
}

int main(int argc, char const* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::string line;
  std::string last_line;
  bool find_delay = false;

  if (args.size() >= 2 && args[1] == "2") {
    find_delay = true;
  }

  std::vector<int> instructions;

  std::vector<int> layers;

  while (std::getline(std::cin, line)) {
    last_line = line;

    std::vector<std::string> words = absl::StrSplit(line, ":");
    if (words.size() != 2) {
      std::cerr << "Malformed input '" << line << "'." << std::endl;
      exit(EXIT_FAILURE);
    }

    int depth;
    int range;

    if (!absl::SimpleAtoi(words[0], &depth)) {
      std::cerr << "Failed to convert '" << words[0] << "' to a number." << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!absl::SimpleAtoi(words[1], &range)) {
      std::cerr << "Failed to convert '" << words[1] << "' to a number." << std::endl;
      exit(EXIT_FAILURE);
    }

    while (layers.size() < depth) {
      layers.emplace_back(0);
    }

    layers.emplace_back(range);
  }

  printf("Total number of layers: %zd\r\n", layers.size());
  printf("last line: %s\r\n", last_line.c_str());

  if (!find_delay) {
    uint32_t total_severity = find_severity(layers, 0);
    printf("Total severity = %d.\r\n", total_severity);
  } else {
    uint32_t delay = 0;

    std::vector<int> scanners(layers.size(), 0);
    std::vector<int> scanner_directions(layers.size(), 1);
    do {
      if (!(delay % 1000)) {
        printf("delay %d.\r\n", delay);
      }

      if (!find_severity2(layers, scanners, scanner_directions)) {
        break;
      }

      scanners = move_scanners(layers, scanners, &scanner_directions);
      delay++;
    } while (1);

    printf("Went through without getting caught when delaying %d picoseconds.\r\n", delay);
  }
}
