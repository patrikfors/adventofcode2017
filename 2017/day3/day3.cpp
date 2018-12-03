#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "absl/strings/numbers.h"

using point = std::tuple<int, int>;

enum class Direction { R, U, L, D };

point move(point const& at, Direction const& direction) {
  point new_position(at);

  switch (direction) {
    case Direction::R:
      std::get<0>(new_position) = std::get<0>(at) + 1;
      break;
    case Direction::U:
      std::get<1>(new_position) = std::get<1>(at) - 1;
      break;
    case Direction::L:
      std::get<0>(new_position) = std::get<0>(at) - 1;
      break;
    case Direction::D:
      std::get<1>(new_position) = std::get<1>(at) + 1;
      break;
  }

  return new_position;
}

Direction turn_left(Direction const& d) {
  switch (d) {
    case Direction::R:
      return Direction::U;
    case Direction::U:
      return Direction::L;
    case Direction::L:
      return Direction::D;
    case Direction::D:
      return Direction::R;
  }
  return Direction::R;
}

std::string direction_to_string(Direction const& direction) {
  switch (direction) {
    case Direction::R:
      return "Right";
    case Direction::U:
      return "Up";
    case Direction::L:
      return "Left";
    case Direction::D:
      return "Down";
  }
  return "Invalid";
}

bool can_turn_left(std::set<point> const& visited, point const& at, Direction const& current_direction) {
  Direction new_direction = turn_left(current_direction);

  point new_position = move(at, new_direction);

  return (visited.find(new_position) == visited.end());
}

bool can_turn_left2(std::map<point, uint32_t> const& visited, point const& at, Direction const& current_direction) {
  Direction new_direction = turn_left(current_direction);

  point new_position = move(at, new_direction);

  return (visited.find(new_position) == visited.end());
}

uint32_t get_value(std::map<point, uint32_t> const& visited, point const& at, int dx, int dy) {
  point new_point(std::get<0>(at) + dx, std::get<1>(at) + dy);

  if (visited.find(new_point) != visited.end()) {
    return visited.at(new_point);
  }

  return 0;
}

uint32_t get_sum(std::map<point, uint32_t> const& visited, point const& at) {
  uint32_t acc = 0;

  acc += get_value(visited, at, -1, -1);
  acc += get_value(visited, at, 0, -1);
  acc += get_value(visited, at, 1, -1);
  acc += get_value(visited, at, -1, 0);
  acc += get_value(visited, at, 1, 0);
  acc += get_value(visited, at, -1, 1);
  acc += get_value(visited, at, 0, 1);
  acc += get_value(visited, at, 1, 1);

  return acc;
}

int main(int argc, char const* argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::string line;

  while (std::getline(std::cin, line)) {
    uint32_t input;
    if (!absl::SimpleAtoi(line, &input)) {
      std::cerr << "Failed to convert '" << line << "' to a number." << std::endl;
      continue;
    }

    if (args.size() >= 2 && args[1] == "2") {
      std::map<point, uint32_t> visited;

      uint32_t steps = 0;
      point at(0, 0);
      uint32_t v = 1;
      Direction direction = Direction::D;

      uint32_t sum = 0;

      while (sum <= input) {
        v++;

        if (at == point(0, 0)) {
          sum = 1;
        } else {
          sum = get_sum(visited, at);
        }

        visited[at] = sum;

        if (can_turn_left2(visited, at, direction)) {
          direction = turn_left(direction);
        }

        at = move(at, direction);
      }

      printf("Finished with sum = %d\r\n", sum);

    } else {
      std::set<point> visited;

      uint32_t steps = 0;
      point at(0, 0);
      uint32_t v = 1;
      Direction direction = Direction::D;

      while (v < input) {
        v++;
        visited.insert(at);

        if (can_turn_left(visited, at, direction)) {
          direction = turn_left(direction);
        }
        at = move(at, direction);
      }

      printf("Finished at (%d,%d). Distance is %d\r\n", std::get<0>(at), std::get<1>(at), abs(std::get<0>(at)) + abs(std::get<1>(at)));
    }
  }
}
