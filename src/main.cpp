#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "compare.h"

namespace fs = std::filesystem;

std::string readBinaryFile(const std::string& fileName) {
  unsigned int size = 0;
  std::ifstream infile(fileName, std::ifstream::binary);
  infile.read(reinterpret_cast<char*>(&size), sizeof(size));
  std::string buffer;
  buffer.resize(size);
  infile.read(&buffer[0], size);
  infile.close();
  return buffer;
}

class Solver {
 public:
  void GetArgv(char** argv) {
    for (const auto& entry: fs::directory_iterator(argv[1])) {
      first_dir_file_names.push_back(entry.path());
    }

    for (const auto& entry: fs::directory_iterator(argv[2])) {
      second_dir_file_names.push_back(entry.path());
    }

    first_to_second.assign(first_dir_file_names.size(), false);
    second_to_first.assign(second_dir_file_names.size(), false);

    low_bound = std::stof(argv[3]);
  }

  void Solve() {
    for (int first_dir_index = 0; first_dir_index < first_dir_file_names.size(); ++first_dir_index) {
      std::string first_file_str = readBinaryFile(first_dir_file_names[first_dir_index]);
      for (int second_dir_index = 0; second_dir_index < second_dir_file_names.size(); ++second_dir_index) {
        std::string second_file_str = readBinaryFile(second_dir_file_names[second_dir_index]);
        float similarity = compare(first_file_str, second_file_str);
        if (similarity == 1) {
          identical.emplace_back(first_dir_index, second_dir_index);
          first_to_second[first_dir_index] = true;
          second_to_first[second_dir_index] = true;
        } else if (similarity >= low_bound) {
          similar.push_back({{first_dir_index, second_dir_index}, similarity});
        }
      }
    }
  }

  void Output() {
    if (!identical.empty()) { std::cout << "Identical files:\n"; }
    for (const auto& [first_index, second_index]: identical) {
      std::cout << first_dir_file_names[first_index] << " - " <<
              second_dir_file_names[second_index] << '\n';
    }
    if (!similar.empty()) { std::cout << "Similar files:\n"; }
    for (const auto& [p, percentage]: similar) {
      std::cout << first_dir_file_names[p.first] << " - " <<
              second_dir_file_names[p.second] << " - " << percentage << '\n';
    }
    bool printed_message = false;
    for (int i = 0; i < first_to_second.size(); ++i) {
      if (!first_to_second[i]) {
        if (!printed_message) {
          std::cout << "In first, but not in second:\n";
          printed_message = true;
        }
        std::cout << first_dir_file_names[i] << '\n';
      }
    }
    printed_message = false;
    for (int i = 0; i < second_to_first.size(); ++i) {
      if (!second_to_first[i]) {
        if (!printed_message) {
          std::cout << "In second, but not in first:\n";
          printed_message = true;
        }
        std::cout << second_dir_file_names[i] << '\n';
      }
    }

  }

 private:
  std::vector<std::string> first_dir_file_names;
  std::vector<std::string> second_dir_file_names;

  std::vector<std::pair<int, int>> identical;
  std::vector<std::pair<std::pair<int, int>, float>> similar;

  // 1 if file has identical in the second directory, 0 otherwise
  std::vector<bool> first_to_second;

  // 1 if file has identical in the first directory, 0 otherwise
  std::vector<bool> second_to_first;

  float low_bound;
};


int main(int argc, char** argv) {
  Solver solver;
  solver.GetArgv(argv);
  solver.Solve();
  solver.Output();
}
