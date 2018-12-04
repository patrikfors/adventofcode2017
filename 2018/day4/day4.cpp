#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

std::vector<std::string> read_input() {
  std::vector<std::string> rval;

  std::string line;
  while (std::getline(std::cin, line)) {
    rval.emplace_back(line);
  }

  return rval;
}

std::vector<std::string> sort_schedule(std::vector<std::string> schedule) {
  // [1518-11-04 00:46] wakes up
  std::regex date_pattern(
      "^\\[(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d) (\\d\\d):(\\d\\d)\\]",
      std::regex_constants::ECMAScript);
  auto compareScheduleEntry = [date_pattern](std::string a, std::string b) {
    int32_t a_year;
    int32_t a_month;
    int32_t a_day;
    int32_t a_hour;
    int32_t a_minute;
    int32_t b_year;
    int32_t b_month;
    int32_t b_day;
    int32_t b_hour;
    int32_t b_minute;

    std::smatch matches;
    if (std::regex_search(a, matches, date_pattern)) {
      a_year = std::stoi(matches[1].str());
      a_month = std::stoi(matches[2].str());
      a_day = std::stoi(matches[3].str());
      a_hour = std::stoi(matches[4].str());
      a_minute = std::stoi(matches[5].str());
    } else {
      std::cout << "no match in a!";
      return false;
    }

    if (std::regex_search(b, matches, date_pattern)) {
      b_year = std::stoi(matches[1].str());
      b_month = std::stoi(matches[2].str());
      b_day = std::stoi(matches[3].str());
      b_hour = std::stoi(matches[4].str());
      b_minute = std::stoi(matches[5].str());
    } else {
      std::cout << "no match in b!";
      return false;
    }

    if (a_year < b_year) {
      return true;
    } else if (a_year > b_year) {
      return false;
    }

    if (a_month < b_month) {
      return true;
    } else if (a_month > b_month) {
      return false;
    }

    if (a_day < b_day) {
      return true;
    } else if (a_day > b_day) {
      return false;
    }

    if (a_hour < b_hour) {
      return true;
    } else if (a_hour > b_hour) {
      return false;
    }

    return (a_minute < b_minute);
  };

  std::sort(schedule.begin(), schedule.end(), compareScheduleEntry);

  return schedule;
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

  int32_t verbose_id = -1;
  int32_t verbose_sum = 0;

  if ((args[1] == "1") || (args[1] == "2")) {
    if (args.size() > 2) {
      verbose_id = std::stoi(args[2]);
    }

    std::vector<std::string> schedule = sort_schedule(read_input());

    std::string id_sleeping[10000];
    std::int32_t id_minutes[10000];

    for (size_t i = 0; i < 10000; i++) {
      id_sleeping[i] = std::string(60, 100);
      id_minutes[i] = 0;
    }

    std::regex begins_pattern("^\\[\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d:\\d\\d\\] "
                              "Guard #([\\d]+) begins shift",
                              std::regex_constants::ECMAScript);
    std::regex sleep_pattern(
        "^\\[\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d:(\\d\\d)\\] falls asleep",
        std::regex_constants::ECMAScript);
    std::regex wake_pattern(
        "^\\[\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d:(\\d\\d)\\] wakes up",
        std::regex_constants::ECMAScript);

    int32_t id = -1;
    int32_t sleep = -1;
    int32_t wake = -1;
    for (auto s : schedule) {
      // std::cout << "#" << s << std::endl;
      std::smatch matches;
      bool mark_sleeping = false;
      if (std::regex_search(s, matches, begins_pattern)) {
        if (sleep >= 0) {
          std::cout << "slept until end of shift?" << std::endl;
          // slept until end of shift?
          wake = 60;
          mark_sleeping = true;
        }

        id = std::stoi(matches[1].str());
        if (id == verbose_id) {
          std::cout << s << std::endl;
        }
      } else if (std::regex_search(s, matches, sleep_pattern)) {
        if (id == verbose_id) {
          std::cout << s << std::endl;
        }

        sleep = std::stoi(matches[1].str());
      } else if (std::regex_search(s, matches, wake_pattern)) {
        if (id == verbose_id) {
          std::cout << s << std::endl;
        }

        wake = std::stoi(matches[1].str());
        mark_sleeping = true;
      } else {
        std::cout << "\"" << s << "\" didn't match." << std::endl;
      }

      if (mark_sleeping) {
        id_minutes[id] += (wake - sleep);
        if (id == verbose_id) {
          std::cout << "Guard #" << id << " slept between " << sleep << " and "
                    << wake << std::endl;
          verbose_sum += (wake - sleep);
        }
        for (int32_t i = sleep; i < wake; i++) {
          if (id_sleeping[id][i] == 100) {
            id_sleeping[id][i] = 1;
          } else {
            id_sleeping[id][i] = id_sleeping[id][i] + 1;
          }
        }

        wake = -1;
        sleep = -1;

        mark_sleeping = false;
      }
    }

    std::int32_t most_sleeping = -1;
    int32_t most_sleeping_guard = -1;

    std::cout << "sleep analysis:" << std::endl;

    for (size_t i = 0; i < 10000; i++) {
      if (id_minutes[i] > most_sleeping) {
        std::cout << "Guard #" << i << " slept " << id_minutes[i] << " minutes."
                  << std::endl;
        most_sleeping = id_minutes[i];
        most_sleeping_guard = i;
      }
    }

    int32_t max_minute = -1;
    int32_t max_value = -1;
    for (size_t i = 0; i < 60; i++) {
      int32_t value = id_sleeping[most_sleeping_guard][i];
      if (value == 100) {
        value = 0;
      }
      std::cout << "Minute " << i << " " << value << std::endl;
      if (value > max_value) {
        max_value = value;
        max_minute = i;
      }
    }

    std::cout << "Guard #" << most_sleeping_guard
              << " slept most, and most often at minute " << max_minute
              << std::endl;

    std::cout << "guard id x minute = " << (most_sleeping_guard * max_minute)
              << std::endl;
    if (verbose_sum > 0) {
      std::cout << "Test sum: " << verbose_sum << std::endl;
    }

    max_value = 0;
    int32_t max_id = 0;
    ;
    max_minute = 0;
    for (size_t id = 0; id < 10000; id++) {
      for (size_t minute = 0; minute < 60; minute++) {
        int32_t value = id_sleeping[id][minute];
        if (value == 100) {
          value = 0;
        }

        if (value > max_value) {
          max_value = value;
          max_id = id;
          max_minute = minute;
        }
      }
    }

    std::cout << "Guard #" << max_id << " spent minute " << max_minute
              << " asleep " << max_value << " times." << std::endl;
    std::cout << "Guard id x minute = " << (max_id * max_minute) << std::endl;

  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
