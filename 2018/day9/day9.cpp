#include <cstdint>
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
    // 10 players; last marble is worth 1618 points: high score is 8317
    static const uint32_t no_players = 476;
    static const uint32_t last_marble_worth = 7165700;//1104;//1618;

    uint32_t scores[no_players + 1];
for (size_t p = 1; p <= no_players; p++) {
    scores[p] = 0;
}

    uint32_t marble_worth = 0;
    std::vector<uint32_t> marbles;

    size_t current_marble;

    uint32_t player = 0;
    while (marble_worth <= last_marble_worth) {
      if (player == 0) {
        marbles.emplace_back(marble_worth);
        current_marble = 0;
        marble_worth++;
        std::cout << "Starting with marble 0." << std::endl;
      } else {
        if (!(marble_worth % 23)) {
          // However, if the marble that is about to be placed has a number
          // which is a multiple of 23, something entirely different happens.
          // First, the current player keeps the marble they would have placed,
          // adding it to their score. In addition, the marble 7 marbles
          // counter-clockwise from the current marble is removed from the
          // circle and also added to the current player's score. The marble
          // located immediately clockwise of the marble that was removed
          // becomes the new current marble.
          scores[player] += marble_worth;
          size_t remove_idx =
              (current_marble + marbles.size() - 7) % marbles.size();
          // std::cout << "current_marble=" << current_marble;
          // std::cout << ", remove idx=" << remove_idx << std::endl << std::flush;
          
          scores[player] += marbles[remove_idx];
          marbles.erase(marbles.begin() + remove_idx);
          current_marble = remove_idx % marbles.size();
          marble_worth++;
        } else {
          size_t insertion_idx = current_marble;

          insertion_idx = (insertion_idx + 2) % marbles.size();

          // insert new marble
          if (insertion_idx == 0) {
          //  std::cout << "push_back" << std::endl << std::flush;
            marbles.push_back(marble_worth);
            current_marble = marbles.size() - 1;
          } else {
          //  std::cout << "insert" << std::endl << std::flush;
            marbles.insert(marbles.begin() + insertion_idx, marble_worth);
            current_marble = insertion_idx;
          }

          //std::cout << "Placed marble " << marble_worth << "." << std::endl;
          marble_worth++;
        }
      }

/*
      if (!player) {
        std::cout << "[-]   ";
      } else {
        std::cout << "[" << player << "]   ";
      }

      for (size_t i = 0; i < marbles.size(); i++) {
        if (i == current_marble)
          {std::cout << "(" << marbles[i] << ") ";}
        else
          {std::cout << marbles[i] << " ";}
      }

      std::cout << std::endl;
      */

      player++;
      if (player > no_players) {
        player = 1;
      }
    }

    uint32_t high_score = 0;
    for (size_t p = 1; p <= no_players; p++) {
      if (scores[p] > high_score) {
        high_score = scores[p];
        std::cout << "Player " << p
                  << " beats previous best with a new high score of "
                  << high_score << std::endl;
      }
    }
  } else if (args[1] == "2") {
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
