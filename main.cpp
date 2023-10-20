#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string readFile(const std::string& fileName) {
  unsigned int size = 0;
  std::ifstream infile(fileName, std::ifstream::binary);
  infile.read(reinterpret_cast<char *>(&size), sizeof(size));
  std::string buffer;
  buffer.resize(size);
  infile.read(&buffer[0], size);
  infile.close();
  return buffer;
}

class Solver {
 public:
  void GetNames(char** argv) {

    for (const auto& entry: fs::directory_iterator(argv[1])) {
      first_dir_file_names.push_back(entry.path());
    }

    for (const auto& entry: fs::directory_iterator(argv[2])) {
      second_dir_file_names.push_back(entry.path());
    }
  }

  void Solve() {

  }

 private:
  std::vector<std::string> first_dir_file_names;
  std::vector<std::string> second_dir_file_names;
};


int main(int argc, char** argv) {
  Solver solver;
  solver.GetNames(argv);

}
