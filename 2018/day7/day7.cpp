#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <set>
#include <vector>

std::vector<std::string> read_input() {
  std::vector<std::string> rval;

  std::string line;
  while (std::getline(std::cin, line)) {
    rval.emplace_back(line);
  }

  return rval;
}

using steps_t = std::map<char, std::string>;

steps_t parse_input(std::vector<std::string> const input) {
  static std::regex step_pattern(
      "^Step ([A-Z]) must be finished before step ([A-Z]) can begin\\.",
      std::regex_constants::ECMAScript);

  steps_t rval;

  for (std::string const step_text : input) {
    std::smatch matches;
    if (std::regex_search(step_text, matches, step_pattern)) {
      char requirement = matches[1].str()[0];
      char step = matches[2].str()[0];

      if (rval.count(step)) {
        std::string requirements = rval[step] + requirement;
        std::sort(requirements.begin(), requirements.end());
        rval[step] = requirements;
      } else {
        std::string requirements = std::string() + requirement;
        rval.emplace(step, requirements);
      }

      // also add the requirement as a step if it doesnt exist already
      if (!rval.count(requirement)) {
        rval.emplace(requirement, std::string());
      }
    } else {
      std::cerr << "Failed to parse step text \"" << step_text << "\"."
                << std::endl;
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

  if((args[1] == "2") && args.size() < 4) {
    arg_fail = true;
  }

  if (arg_fail) {
    std::cout << "Usage: " << args[0] << " [1|2 <workers> <work_time>]" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<std::string> input = read_input();
  steps_t steps = parse_input(input);

  if (args[1] == "1") {
    std::string result;

    while (steps.size()) {
      std::cout << std::endl;
      std::string finished_steps;
      std::cout << "loop:" << std::endl;

      for (auto it = steps.begin(); it != steps.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
      }

      for (auto it = steps.begin(); it != steps.end(); ++it) {
        bool finished = true;
        for (char c : it->second) {
          if (steps.count(c)) {
            finished = false;
            break;
          }
        }

        if (finished) {
          char c = it->first;
          steps.erase(c);

          result += c;
          break;
        }
      }

      std::cout << "Execution order: " << result << std::endl;
    }
  } else if (args[1] == "2") {
    int workers = std::stoi(args[2]);
    int work_time = std::stoi(args[3]);
    std::string result;
    int work_left[26];

    for(int i=0;i<26;i++) {
      //work_left[i] = i + 1;
      work_left[i] = work_time + i + 1;
    }

    int second = 0;

    std::set<char> started_work; // looks like we have to finish started work first;
    bool print=true;
    while (steps.size()) {
      std::cout << std::endl;
      std::cout << "second " << second++ << ":" << std::endl;

      std::string available_steps;
      int available_workers = workers;

      if (print) {
        for (auto it = steps.begin(); it != steps.end(); ++it) {
          std::cout << "[" << std::setw(3) << work_left[(it->first-'A')] << "]" << it->first << ": " << it->second << std::endl;
        }
        print = false;
      }

      for (auto it = steps.begin(); it != steps.end(); ++it) {
        // see if step (= it->first) is ready to work on
        bool ready_to_work_on = true;
        for (char c : it->second) {
          if (steps.count(c)) {
            ready_to_work_on = false;
            break;
          }
        }

        if(ready_to_work_on) {
          available_steps += it->first;
        }
      }

      std::cout << "Available steps: " << available_steps << std::endl;

      if(started_work.size()) {
        // continue started work
          for(char c : started_work) {
            std::cout << "Worker " << (1+workers-available_workers) << " continues works 1 second on step " << c << std::endl;
            available_workers--;
            work_left[c-'A']--;
          }
      } 
      // ready to work 1 second
      for(char c : available_steps) {
        if (started_work.count(c)) {
          continue;
        }
        if(available_workers) {
          started_work.insert(c);
          std::cout << "Worker " << (1+workers-available_workers) << " works 1 second on step " << c << std::endl;
          available_workers--;
          work_left[c-'A']--;
        }
      }


      // remove finished work
      for(int i=0;i<26;i++) {
        if(work_left[i] == 0) {
          print = true;
          char c = 'A' + i;
          started_work.erase(c);
          std::cout << "Removing finished work " << c << std::endl;
          work_left[i] = -1; // mark it as removed
          steps.erase(c);
        }
      }

    }
   std::cout << "Time required: " << second << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
