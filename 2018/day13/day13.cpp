#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <set>
#include <vector>

#include <fmt/core.h>
#include <gflags/gflags.h>

DEFINE_bool(v, false, "Be verbose");
DEFINE_int32(part, 0, "Problem part");
DEFINE_string(f, "", "Input data");

static const size_t width = 200;
static const size_t height = 200;

static bool part_validator(char const *flagname, int32_t value) {
  return (value > 0);
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

std::string get_initial_state(std::vector<std::string> const &data) {
  static const std::string pattern_string = "^initial state: ([#\\.]+)";
  static std::regex pattern(pattern_string, std::regex_constants::ECMAScript);

  for (auto s : data) {
    std::smatch matches;
    if (std::regex_search(s, matches, pattern)) {
      return matches[1].str();
    }
  }

  return "FAILED!";
}

int get_plant_mask(std::vector<std::string> const &data, bool mask[32]) {
  static const std::string pattern_string = "^([#\\.]+) => #";
  static std::regex pattern(pattern_string, std::regex_constants::ECMAScript);

  for (size_t i = 0; i < 32; i++) {
    mask[i] = false;
  }

  for (auto line : data) {
    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
      std::string bits_str = matches[1].str();
      size_t bits = bits_str.size();
      uint8_t idx = 0;

      for (size_t i = 0; i < bits; i++) {
        if (bits_str[i] == '#') {
          idx |= (1 << (bits - i - 1));
        }
      }

      mask[idx] = true;

      if (FLAGS_v) {
        fmt::print("bit string {} => bits = {}.\n", bits_str, idx);
      }
    } else if (FLAGS_v) {
      fmt::print("skipping input \"{}\".\n", line);
    }
  }

  return 0;
}

struct node_t {
  uint8_t bits;
  uint8_t next_turn;  // 0 = left, 1 = straight, 2 = right
  uint8_t cart_id;
};

// using node_t = uint8_t;
static const uint8_t BIT_CONNECTED_LEFT = (1 << 3);
static const uint8_t BIT_CONNECTED_RIGHT = (1 << 2);
static const uint8_t BIT_CONNECTED_UP = (1 << 1);
static const uint8_t BIT_CONNECTED_DOWN = (1 << 0);
static const uint8_t BIT_CONNECTED_MASK = 0xf;
static const uint8_t BIT_CART_LEFT = (1 << 7);
static const uint8_t BIT_CART_RIGHT = (1 << 6);
static const uint8_t BIT_CART_UP = (1 << 5);
static const uint8_t BIT_CART_DOWN = (1 << 4);
static const uint8_t BIT_CART_MASK = 0xf0;

inline static bool is_connected_left(node_t node) {
  return (node.bits & BIT_CONNECTED_LEFT);
}
inline static bool is_connected_right(node_t node) {
  return (node.bits & BIT_CONNECTED_RIGHT);
}
inline static bool is_connected_up(node_t node) {
  return (node.bits & BIT_CONNECTED_UP);
}
inline static bool is_connected_down(node_t node) {
  return (node.bits & BIT_CONNECTED_DOWN);
}
inline static bool is_cart(node_t node) { return (node.bits & BIT_CART_MASK); }
inline static bool is_cart_left(node_t node) {
  return (node.bits & BIT_CART_LEFT);
}
inline static bool is_cart_right(node_t node) {
  return (node.bits & BIT_CART_RIGHT);
}
inline static bool is_cart_up(node_t node) { return (node.bits & BIT_CART_UP); }
inline static bool is_cart_down(node_t node) {
  return (node.bits & BIT_CART_DOWN);
}

inline static bool is_turn(node_t node) {
  uint8_t connections = (node.bits & BIT_CONNECTED_MASK);

  return (
      (connections == (BIT_CONNECTED_UP | BIT_CONNECTED_RIGHT))    //  a \ turn
      || (connections == (BIT_CONNECTED_UP | BIT_CONNECTED_LEFT))  //  a / turn
      ||
      (connections == (BIT_CONNECTED_DOWN | BIT_CONNECTED_LEFT))  // a \ turn 2
      ||
      (connections == (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT))  // a / turn 2
  );
}
inline static bool is_crossing(node_t node) {
  uint8_t connections = (node.bits & BIT_CONNECTED_MASK);

  if (is_turn(node)) {
    return false;
  }

  return (
      (connections != 0)  // is connected somewhere
      &&
      (connections != (BIT_CONNECTED_LEFT |
                       BIT_CONNECTED_RIGHT))  // not a simple horizontal track
      && (connections != (BIT_CONNECTED_UP |
                          BIT_CONNECTED_DOWN))  // not a simple vertical track
  );
}

inline static void set_connected_left(node_t &node) {
  node.bits |= BIT_CONNECTED_LEFT;
}
inline static void set_connected_right(node_t &node) {
  node.bits |= BIT_CONNECTED_RIGHT;
}
inline static void set_connected_up(node_t &node) {
  node.bits |= BIT_CONNECTED_UP;
}
inline static void set_connected_down(node_t &node) {
  node.bits |= BIT_CONNECTED_DOWN;
}
inline static void set_cart_left(node_t &node) { node.bits |= BIT_CART_LEFT; }
inline static void set_cart_right(node_t &node) { node.bits |= BIT_CART_RIGHT; }
inline static void set_cart_up(node_t &node) { node.bits |= BIT_CART_UP; }
inline static void set_cart_down(node_t &node) { node.bits |= BIT_CART_DOWN; }
inline static void clear_cart_left(node_t &node) {
  node.bits &= ~BIT_CART_LEFT;
}
inline static void clear_cart_right(node_t &node) {
  node.bits &= ~BIT_CART_RIGHT;
}
inline static void clear_cart_up(node_t &node) { node.bits &= ~BIT_CART_UP; }
inline static void clear_cart_down(node_t &node) {
  node.bits &= ~BIT_CART_DOWN;
}
inline static void clear_cart(node_t &node) { node.bits &= ~BIT_CART_MASK; }
inline static void set_cart(node_t &node, uint8_t cart_bits) {
  node.bits |= cart_bits;
}

inline static char graph_character(node_t node) {
  if (node.bits & BIT_CART_DOWN) return 'v';
  if (node.bits & BIT_CART_UP) return '^';
  if (node.bits & BIT_CART_LEFT) return '<';
  if (node.bits & BIT_CART_RIGHT) return '>';

  switch (node.bits) {
    case (BIT_CONNECTED_LEFT | BIT_CONNECTED_RIGHT):
      return '-';
    case (BIT_CONNECTED_UP | BIT_CONNECTED_DOWN):
      return '|';
    case (BIT_CONNECTED_UP | BIT_CONNECTED_RIGHT):
    case (BIT_CONNECTED_LEFT | BIT_CONNECTED_DOWN):
      return '\\';
    case (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT):
    case (BIT_CONNECTED_UP | BIT_CONNECTED_LEFT):
      return '//';
    case (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT | BIT_CONNECTED_UP |
          BIT_CONNECTED_LEFT):
      return '+';
    case (BIT_CONNECTED_LEFT | BIT_CONNECTED_RIGHT | BIT_CART_LEFT):
    case (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT | BIT_CONNECTED_UP |
          BIT_CONNECTED_LEFT | BIT_CART_LEFT):
      return '<';
    case (BIT_CONNECTED_LEFT | BIT_CONNECTED_RIGHT | BIT_CART_RIGHT):
    case (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT | BIT_CONNECTED_UP |
          BIT_CONNECTED_LEFT | BIT_CART_RIGHT):
      return '>';
    case (BIT_CONNECTED_UP | BIT_CONNECTED_DOWN | BIT_CART_UP):
    case (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT | BIT_CONNECTED_UP |
          BIT_CONNECTED_LEFT | BIT_CART_UP):
      return '^';
    case (BIT_CONNECTED_UP | BIT_CONNECTED_DOWN | BIT_CART_DOWN):
    case (BIT_CONNECTED_DOWN | BIT_CONNECTED_RIGHT | BIT_CONNECTED_UP |
          BIT_CONNECTED_LEFT | BIT_CART_DOWN):
      return 'v';
    case 0:
      return ' ';
    default:
      return '#';
  }
}

bool is_connected_to(node_t node, int dx, int dy) {
  if (dx > 0)
    return is_connected_right(node);
  else if (dx < 0)
    return is_connected_left(node);
  else if (dy > 0)
    return is_connected_down(node);
  else if (dy < 0)
    return is_connected_up(node);
  else
    assert(!"bad delta direction");
}

void get_cart_direction(node_t node, int &dx, int &dy) {
  if (is_cart_down(node)) {
    dx = 0;
    dy = 1;
  } else if (is_cart_left(node)) {
    dx = -1;
    dy = 0;
  } else if (is_cart_up(node)) {
    dx = 0;
    dy = -1;
  } else if (is_cart_right(node)) {
    dx = 1;
    dy = 0;
  } else
    assert(!"not a cart");
}

int parse_input(std::vector<std::string> input, node_t nodes[width][height]) {
  uint8_t cart_id = 1;

  for (size_t y = 0; y < input.size(); y++) {
    std::string row = input[y];
    for (size_t x = 0; x < row.size(); x++) {
      switch (row[x]) {
        case '/':
          if (x == 0 || !is_connected_right(nodes[x - 1][y])) {
            set_connected_right(nodes[x][y]);
            set_connected_down(nodes[x][y]);
          } else {
            set_connected_left(nodes[x][y]);
            set_connected_up(nodes[x][y]);
          }
          break;

        case '\\':
          if (x == 0 || (y > 0 && is_connected_down(nodes[x][y - 1]))) {
            set_connected_right(nodes[x][y]);
            set_connected_up(nodes[x][y]);
          } else {
            set_connected_left(nodes[x][y]);
            set_connected_down(nodes[x][y]);
          }
          break;

        case '+':
          set_connected_left(nodes[x][y]);
          set_connected_down(nodes[x][y]);
          set_connected_right(nodes[x][y]);
          set_connected_up(nodes[x][y]);
          break;

        case '|':
          set_connected_down(nodes[x][y]);
          set_connected_up(nodes[x][y]);
          break;

        case 'v':
          set_connected_down(nodes[x][y]);
          set_connected_up(nodes[x][y]);
          set_cart_down(nodes[x][y]);
          nodes[x][y].cart_id = cart_id++;
          break;

        case '^':
          set_connected_down(nodes[x][y]);
          set_connected_up(nodes[x][y]);
          set_cart_up(nodes[x][y]);
          nodes[x][y].cart_id = cart_id++;
          break;

        case '-':
          set_connected_left(nodes[x][y]);
          set_connected_right(nodes[x][y]);
          break;

        case '<':
          set_connected_left(nodes[x][y]);
          set_connected_right(nodes[x][y]);
          set_cart_left(nodes[x][y]);
          nodes[x][y].cart_id = cart_id++;
          break;

        case '>':
          set_connected_left(nodes[x][y]);
          set_connected_right(nodes[x][y]);
          set_cart_right(nodes[x][y]);
          nodes[x][y].cart_id = cart_id++;
          break;

        case ' ':
          break;

        default:
          fmt::print("Unexpected character '{}'.\n", row[x]);
      }
    }
  }

  return cart_id - 1;
}

void print_track(node_t nodes[width][height]) {
  for (size_t y = 0; y < width; y++) {
    bool empty_row = true;
    std::string row = "";
    for (size_t x = 0; x < height; x++) {
      char c = graph_character(nodes[x][y]);
      if (c != ' ') {
        empty_row = false;
      }

      row += c;
    }

    if (!empty_row) {
      fmt::print("print Row {:3}: {}\r", y, row);
    }
  }
}

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("[-v] [-f <input>  -part <1|2>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // positional arguments
  std::vector<std::string> args(argv, argv + argc);

  std::ios_base::sync_with_stdio(false);

  std::vector<std::string> input;
  if (FLAGS_f.size() > 0) {
    std::ifstream file(FLAGS_f, std::ifstream::in);
    input = read_input(file);
  } else {
    input = read_input(std::cin);
  }

  node_t nodes[width][height];

  memset(nodes, 0, width * height * sizeof(node_t));

  int carts = parse_input(input, nodes);

  if (FLAGS_v) {
    fmt::print("Parsing complete.\n");
    fmt::print("{} carts", carts);
    std::cout << "Press Enter to print." << std::flush;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    print_track(nodes);
  }

  using point_t = std::pair<size_t, size_t>;

  for (int i = 0; carts > 1; i++) {
    bool done = false;

    std::set<point_t> skip_points;
    skip_points.clear();
    for (size_t y = 0; y < width; y++) {
      for (size_t x = 0; x < height; x++) {
        point_t p = point_t(x, y);
        if (skip_points.find(p) != skip_points.end()) {
          continue;
        }

        if (is_cart(nodes[x][y])) {
          int dx;
          int dy;
          uint8_t cart_bit = nodes[x][y].bits & BIT_CART_MASK;

          get_cart_direction(nodes[x][y], dx, dy);

          if (is_turn(nodes[x][y])) {
            uint8_t connections = nodes[x][y].bits & BIT_CONNECTED_MASK;
            if (cart_bit == BIT_CART_RIGHT)
              connections &= (~BIT_CONNECTED_LEFT);
            else if (cart_bit == BIT_CART_DOWN)
              connections &= (~BIT_CONNECTED_UP);
            else if (cart_bit == BIT_CART_LEFT)
              connections &= (~BIT_CONNECTED_RIGHT);
            else if (cart_bit == BIT_CART_UP)
              connections &= (~BIT_CONNECTED_DOWN);
            else
              assert(!"bah!");

            if (connections & BIT_CONNECTED_DOWN) {
              cart_bit = BIT_CART_DOWN;
              dx = 0;
              dy = 1;
            }
            if (connections & BIT_CONNECTED_LEFT) {
              cart_bit = BIT_CART_LEFT;
              dx = -1;
              dy = 0;
            }
            if (connections & BIT_CONNECTED_UP) {
              cart_bit = BIT_CART_UP;
              dx = 0;
              dy = -1;
            }
            if (connections & BIT_CONNECTED_RIGHT) {
              cart_bit = BIT_CART_RIGHT;
              dx = 1;
              dy = 0;
            }
          }

          else if (is_crossing(nodes[x][y])) {
            int new_dx;
            int new_dy;
            uint8_t new_cart_bit;
            uint8_t next_turn = nodes[x][y].next_turn;

            if (next_turn == 0) {
              if (cart_bit == BIT_CART_RIGHT) {
                new_cart_bit = BIT_CART_UP;
                new_dx = 0;
                new_dy = -1;
              } else if (cart_bit == BIT_CART_LEFT) {
                new_cart_bit = BIT_CART_DOWN;
                new_dx = 0;
                new_dy = 1;
              } else if (cart_bit == BIT_CART_UP) {
                new_cart_bit = BIT_CART_LEFT;
                new_dx = -1;
                new_dy = 0;
              } else if (cart_bit == BIT_CART_DOWN) {
                new_cart_bit = BIT_CART_RIGHT;
                new_dx = 1;
                new_dy = 0;
              } else
                assert(!"invalid direction");
            } else if (next_turn == 2) {
              if (cart_bit == BIT_CART_RIGHT) {
                new_cart_bit = BIT_CART_DOWN;
                new_dx = 0;
                new_dy = 1;
              } else if (cart_bit == BIT_CART_LEFT) {
                new_cart_bit = BIT_CART_UP;
                new_dx = 0;
                new_dy = -1;
              } else if (cart_bit == BIT_CART_UP) {
                new_cart_bit = BIT_CART_RIGHT;
                new_dx = 1;
                new_dy = 0;
              } else if (cart_bit == BIT_CART_DOWN) {
                new_cart_bit = BIT_CART_LEFT;
                new_dx = -1;
                new_dy = 0;
              } else
                assert(!"invalid direction");
            } else {
              assert(next_turn == 1);
              new_dx = dx;
              new_dy = dy;
              new_cart_bit = cart_bit;
            }

            if (is_connected_to(nodes[x][y], new_dx, new_dy)) {
              // turn ok
              dx = new_dx;
              dy = new_dy;
              cart_bit = new_cart_bit;
              if (FLAGS_v) {
                fmt::print("cart {} using turn {} ", nodes[x][y].cart_id,
                           nodes[x][y].next_turn);
              }
              nodes[x][y].next_turn += 1;
              nodes[x][y].next_turn %= 3;
              if (FLAGS_v) {
                fmt::print("next turn is now {}.\n", nodes[x][y].next_turn);
              }
            } else {
              fmt::print("x={}, y={}\n", x, y);
              assert(!"cant go there");
            }
          }

          // ok, now we know where the cart's going

          if (is_cart(nodes[x + dx][y + dy])) {
            fmt::print("collision at ({}, {})!\n", x + dx, y + dy);

            if (FLAGS_part == 1) {
              done = true;
              break;
            }

            carts -= 2;
            clear_cart(nodes[x][y]);
            clear_cart(nodes[x + dx][y + dy]);
            fmt::print("{} carts left.\n", carts);
            continue;
          }

          // move it
          //  remove from current
          clear_cart(nodes[x][y]);

          nodes[x + dx][y + dy].cart_id = nodes[x][y].cart_id;
          nodes[x][y].cart_id = 0;

          nodes[x + dx][y + dy].next_turn = nodes[x][y].next_turn;
          nodes[x][y].next_turn = UINT8_MAX;

          // add to new
          set_cart(nodes[x + dx][y + dy], cart_bit);

          point_t skip_point = point_t(x + dx, y + dy);
          skip_points.insert(skip_point);
        }
      }
    }

    if (FLAGS_part == 2) {
      if (carts == 1) {
        fmt::print("only 1 cart left!\r\n");
        for (int xx = 0; xx < width; xx++) {
          for (int yy = 0; yy < height; yy++) {
            if (is_cart(nodes[xx][yy])) {
              fmt::print("cart is at ({}, {}).\n", xx, yy);
            }
          }
        }
        break;
      }
    }

    if (FLAGS_v) {
      print_track(nodes);

      std::cout << "Press Enter to continue." << std::flush;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (done) {
      break;
    }
  }

  if (FLAGS_part == 1) {
  } else if (FLAGS_part == 2) {
  } else {
    fmt::print(stderr, "Invalid part {}.\n", FLAGS_part);
  }

  std::cout << "Press Enter to quit." << std::flush;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  exit(0);
}
