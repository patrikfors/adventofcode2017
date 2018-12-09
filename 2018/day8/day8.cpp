#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> read_input() {
  std::vector<std::string> rval;

  std::string line;
  while (std::getline(std::cin, line)) {
    rval.emplace_back(line);
  }

  return rval;
}

uint32_t add_metadata(std::vector<std::uint32_t> const &data, size_t &index) {
  uint32_t metadata_sum = 0;

  uint32_t child_nodes = data[index++];
  uint32_t metadata_nodes = data[index++];

  for (size_t c = 0; c < child_nodes; c++) {
    metadata_sum += add_metadata(data, index);
  }

  for (size_t m = 0; m < metadata_nodes; m++) {
    metadata_sum += data[index++];
  }

  return metadata_sum;
}

char node = 'A';

uint32_t get_node_value(std::vector<std::uint32_t> const &data, size_t &index) {
  uint32_t metadata_sum = 0;

  uint32_t child_nodes = data[index++];
  uint32_t metadata_nodes = data[index++];

  char this_node = node++;

  if (child_nodes == 0) {
    for (size_t m = 0; m < metadata_nodes; m++) {
      metadata_sum += data[index++];
    }

    if (this_node == 'A')
      std::cout << "Node " << this_node << "= " << metadata_sum << std::endl;
    return metadata_sum;
  }

  std::vector<uint32_t> child_node_values;

  if (this_node == 'A')
    std::cout << "Node " << this_node << " has " << child_nodes
              << " child nodes." << std::endl;
  if (this_node == 'A')
    std::cout << "Node " << this_node << " has " << metadata_nodes
              << " meta data." << std::endl;
  for (size_t c = 0; c < child_nodes; c++) {
    uint32_t value = get_node_value(data, index);

    if (this_node == 'A')
      std::cout << this_node << ": child[" << c << "] = " << value << std::endl;
    child_node_values.emplace_back(value);
  }

  for (size_t m = 0; m < metadata_nodes; m++) {
    uint32_t metadata = data[index++];
    if (this_node == 'A')
      std::cout << this_node << ": metadata[" << m << "] = " << metadata;
    if (metadata > child_node_values.size()) {
      if (this_node == 'A')
        std::cout << " .. skipping." << std::endl;
      continue;
    }

    if (this_node == 'A') {

    
      std::cout << std::endl;

      std::cout << "metadata =" << metadata << std::endl;
      std::cout << "adding " << child_node_values[metadata-1] << std::endl;
    }
    metadata_sum += child_node_values[metadata-1];

    if (this_node == 'A')
      std::cout << "metadata_sum=" << metadata_sum <<std::endl;
  }

  if (this_node == 'A') {
    for (size_t c = 0; c < child_nodes; c++) {
      std::cout << "child node[" << c << "]=" << child_node_values[c]
                << std::endl;
    }
    for (size_t m = 0; m < metadata_nodes; m++) {
    }
  }

  std::cout << "Node " << this_node << "= " << metadata_sum << std::endl;
  return metadata_sum;
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

  std::vector<uint32_t> data;
  {
    std::string line;
    std::getline(std::cin, line);

    std::istringstream f(line);
    std::string s;
    while (std::getline(f, s, ' ')) {
      data.emplace_back(std::stoi(s));
    }
  }

  if (args[1] == "1") {
    uint32_t metadata_sum = 0;
    size_t index = 0;

    metadata_sum = add_metadata(data, index);

    std::cout << "Sum of meta data: " << metadata_sum << std::endl;
  } else if (args[1] == "2") {
    size_t index = 0;
    uint32_t result = get_node_value(data, index);

    std::cout << "Node value: " << result << std::endl;
  } else {
    std::cout << "really not implemented yet" << std::endl;
    exit(2);
  }

  exit(0);
}
