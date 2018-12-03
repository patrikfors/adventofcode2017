#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

std::pair<std::uint32_t, std::uint32_t> count_two_and_three(std::string str) {
  uint8_t f[26];

  for (size_t i = 0; i < 26; i++) {
    f[i] = 0;
  }
  for (char c : str) {
    f[c - 'a']++;
  }

  std::pair<std::uint32_t, std::uint32_t> rval = {0, 0};

  for (uint8_t i : f) {
    if (i == 2) {
      rval.first = 1;
    } else if (i == 3) {
      rval.second = 1;
    }
  }

  return rval;
}

bool diff_by_one(std::string a, std::string b) {
  bool diff = false;

  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] != b[i]) {
      if (diff) {
        return false;
      }

      diff = true;
    }
  }

  return diff;
}

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
    std::regex pattern("^#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)", std::regex_constants::ECMAScript);
    std::smatch matches;
    std::string line;

    uint8_t cloth[1000][1000];
    memset(cloth, 0, sizeof(uint8_t) * 1000 * 1000);


    // int32_t w=0;
    // int32_t h=0;

    while (std::getline(std::cin, line)) {
//      std::cout << line << ": ";
      if(std::regex_search(line, matches, pattern)) {
        int32_t id = std::stoi(matches[1].str());
//        std::cout << "id " << id << std::endl;
        int32_t left = std::stoi(matches[2].str());
        int32_t top = std::stoi(matches[3].str());
        int32_t width = std::stoi(matches[4].str());
        int32_t height = std::stoi(matches[5].str());
        //std::cout << "id = " << id << std::endl;
        //std::cout << "left = " << left << std::endl;
        //std::cout << "top = " << top << std::endl;
        //std::cout << "width = " << width << std::endl;
        //std::cout << "height = " << height << std::endl;

        //w = std::max(w, left+width);
        //h = std::max(h, top+height);

        for(int32_t x = left;x<(left+width);x++) {
          for(int32_t y = top;y<(top+height);y++) {
            cloth[x][y] = std::min(cloth[x][y]+1, 10);
//            std::cout << "cloth[" << x << "][" << y << "] = " << (int) cloth[x][y] << std::endl;
          }
        }
      }
      else {
        std::cout << "NO!" << std::endl;
        exit(1);
      }

    }


    int32_t c = 0;
    for(int32_t x = 0;x<1000;x++) {
      for(int32_t y = 0;y<1000;y++) {
        if(cloth[x][y] > 1) {
          c++;
        }
      }
    }


    std::cout << c << " square inches overclaimed!" << std::endl;

    //std::cout << "max w = " << w << std::endl;
    //std::cout << "max h = " << h << std::endl;

  } else if (args[1] == "2") {
    std::vector<std::string> strings;
    std::string line;

    while (std::getline(std::cin, line)) {
      strings.emplace_back(line);
    }

    for (int a = 0; a < strings.size(); a++) {
      for (int b = a + 1; b < strings.size(); b++) {
        if (diff_by_one(strings[a], strings[b])) {
          std::cout << "a: " << strings[a] << std::endl;
          std::cout << "b: " << strings[b] << std::endl;

          std::cout << "diff by one!" << std::endl;
          std::cout << "matching: ";
          for(size_t i=0;i<strings[a].size();i++) {
            if(strings[a][i] == strings[b][i]) {
              std::cout << strings[a][i];
            }
          }
          std::cout << std::endl;

        }
      }
    }

    exit(0);
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
