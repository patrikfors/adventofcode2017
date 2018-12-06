#include <algorithm>
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

struct point_t {
  int32_t x;
  int32_t y;
};
using points_t = std::vector<point_t>;

static bool point_compare_x(point_t a, point_t b)
{
  return a.x < b.x;
} 

static bool point_compare_y(point_t a, point_t b)
{
  return a.y < b.y;
} 

std::vector<point_t> parse_input(std::vector<std::string> const input) {
  static std::regex point_pattern("^(\\d+), (\\d+)",
                                  std::regex_constants::ECMAScript);

  std::vector<point_t> rval;

  for (std::string const point_text : input) {
    std::smatch matches;
    if (std::regex_search(point_text, matches, point_pattern)) {
      point_t point;
      point.x = std::stoi(matches[1].str());
      point.y = std::stoi(matches[2].str());
      rval.emplace_back(point);
    } else {
      std::cerr << "Failed to parse point text \"" << point_text << "\"."
                << std::endl;
      exit(1);
    }
  }

  return rval;
}

int32_t manhattan_distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

int32_t sum_of_distances(int32_t x, int32_t y, points_t points)
{
  int32_t sum = 0;

  for(point_t point : points) {
    sum += manhattan_distance(x, y, point.x, point.y);
  }

  return sum;
}

size_t find_closest_index(int32_t x, int32_t y, std::vector<point_t> points) {
  int32_t closest_distance = INT32_MAX;
  size_t closest_index;

  for (size_t idx = 0; idx < points.size(); idx++) {
    int32_t distance = manhattan_distance(x, y, points[idx].x, points[idx].y);
    if (distance < closest_distance) {
      closest_distance = distance;
      closest_index = idx;
    }
  }

  return closest_index;
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

  std::vector<std::string> input = read_input();

  std::vector<point_t> points = parse_input(input);

  // find min/max x & y..
  points_t::iterator result = std::min_element(points.begin(), points.end(), point_compare_x);
  std::int32_t minx = result->x;
  result = std::max_element(points.begin(), points.end(), point_compare_x);
  std::int32_t maxx = result->x;
  result = std::min_element(points.begin(), points.end(), point_compare_y);
  std::int32_t miny = result->y;
  result = std::max_element(points.begin(), points.end(), point_compare_y);
  std::int32_t maxy = result->y;

  if (args[1] == "1") {
    std::set<size_t> ignore_indices;

    for (int32_t x = minx - 10; x < maxx + 10; x++) {
      size_t idx = find_closest_index(x, miny - 10, points);
      ignore_indices.insert(idx);
      idx = find_closest_index(x, maxy + 10, points);
      ignore_indices.insert(idx);
    }

    for (int32_t y = miny - 10; y < maxy + 10; y++) {
      size_t idx = find_closest_index(minx - 10, y, points);
      ignore_indices.insert(idx);
      idx = find_closest_index(maxx + 10, y, points);
      ignore_indices.insert(idx);
    }

    for (size_t idx : ignore_indices) {
      std::cout << "ignoring index " << idx << std::endl;
    }

    std::vector<int32_t> areas(points.size(), 0);
    for (int32_t y = 0; y < maxy; y++) {
      for (int32_t x = 0; x < maxx; x++) {
        size_t min = INT32_MAX;
        size_t min_p = INT32_MAX;
        bool dup = false;

        for (size_t idx = 0; idx < points.size(); idx++) {
          point_t point = points[idx];
          int32_t distance = manhattan_distance(x, y, point.x, point.y);

          if (distance < min) {
            min = distance;
            min_p = idx;
            dup = false;
          } else if (distance == min) {
            dup = true;
          }
        }

        if (!dup) {
          areas[min_p]++;
        }
      }
    }

    // reset ignore points
    for (size_t ignore_idx : ignore_indices) {
      //std::cout << "setting area[" << ignore_idx << "] to 0." << std::endl;
      areas[ignore_idx] = 0;
    }

    // find largest area
    std::vector<int32_t>::iterator largest_area = std::max_element(areas.begin(), areas.end());
/*
    size_t largest_idx = 0;
    int32_t largest = INT32_MIN;

    for (size_t idx = 0; idx < areas.size(); idx++) {
      if (areas[idx] > largest) {
        largest = areas[idx];
        largest_idx = idx;
      }
    }
*/

    std::cout << "Largest area is " << *largest_area << std::endl;

  } else if (args[1] == "2") {
    int32_t size = 0;
    for (int32_t x = minx - 10; x < maxx + 10; x++) {
      for (int32_t y = miny - 10; y < maxy + 10; y++) {
        int32_t distances = sum_of_distances(x, y, points);

        if (distances < 10000) {
//          std::cout << "(" << x << ", " << y << ") is included." << std::endl;
          size++;
        }
      }
    }
    std::cout << "Size of region: " << size << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
