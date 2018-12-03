#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

int main(int argc, char const *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  bool arg_fail = false;

  if (args.size() < 2 || (args[1] != "1" && args[1] != "2")) {
    arg_fail = true;
    //  } else if (args[1] == "2" && args.size() < 3) {
    //    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2]" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (args[1] == "1") {
    std::regex pattern("^#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)",
                       std::regex_constants::ECMAScript);
    std::smatch matches;
    std::string line;

    uint8_t cloth[1000][1000];
    memset(cloth, 0, sizeof(uint8_t) * 1000 * 1000);

    while (std::getline(std::cin, line)) {
      if (std::regex_search(line, matches, pattern)) {
        int32_t id = std::stoi(matches[1].str());
        int32_t left = std::stoi(matches[2].str());
        int32_t top = std::stoi(matches[3].str());
        int32_t width = std::stoi(matches[4].str());
        int32_t height = std::stoi(matches[5].str());

        for (int32_t x = left; x < (left + width); x++) {
          for (int32_t y = top; y < (top + height); y++) {
            cloth[x][y] = std::min(cloth[x][y] + 1, 10);
          }
        }
      } else {
        std::cout << "NO!" << std::endl;
        exit(1);
      }
    }

    int32_t c = 0;
    for (int32_t x = 0; x < 1000; x++) {
      for (int32_t y = 0; y < 1000; y++) {
        if (cloth[x][y] > 1) {
          c++;
        }
      }
    }

    std::cout << c << " square inches overclaimed!" << std::endl;

  } else if (args[1] == "2") {
    std::regex pattern("^#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)",
                       std::regex_constants::ECMAScript);
    std::smatch matches;
    std::string line;

    int32_t cloth[1000][1000];
    memset(cloth, 0, sizeof(int32_t) * 1000 * 1000);


    using id_area_t = std::pair<int32_t, int32_t>; // id, area

    std::vector<id_area_t> ids;

    while (std::getline(std::cin, line)) {
      if (std::regex_search(line, matches, pattern)) {
        int32_t id = std::stoi(matches[1].str());
        int32_t left = std::stoi(matches[2].str());
        int32_t top = std::stoi(matches[3].str());
        int32_t width = std::stoi(matches[4].str());
        int32_t height = std::stoi(matches[5].str());

        ids.push_back(id_area_t(id, width*height));

        for (int32_t x = left; x < (left + width); x++) {
          for (int32_t y = top; y < (top + height); y++) {
            if (cloth[x][y] != 0) {
              cloth[x][y] = -1;
            } else {
              cloth[x][y] = id;
            }
          }
        }
      } else {
        std::cout << "NO!" << std::endl;
        exit(1);
      }
    }

/*
    int32_t c = 0;
    for (int32_t x = 0; x < 1000; x++) {
      for (int32_t y = 0; y < 1000; y++) {
        if (cloth[x][y] < 0) {
          c++;
        }
      }
    }
*/


    for(auto foo : ids) {
      int32_t id = foo.first;
      int32_t area = foo.second;

      int32_t count = 0;
      for (int32_t x = 0; x < 1000; x++) {
        for (int32_t y = 0; y < 1000; y++) {
          if(cloth[x][y] == id) {
            count++;
          }
        }
      }
    
      if (count == area) {
        std::cout << "id " << id << " is untouched by others!" << std::endl;
      }
    }

    //std::cout << c << " square inches overclaimed!" << std::endl;

    exit(0);
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
