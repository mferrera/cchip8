#include <iostream>
#include <string>

void usage() { std::cout << "Usage: cchip8 some.rom" << std::endl; }

int main(int argc, char** argv) {
  if (argc < 2) {
    usage();
    return 0;
  }

  std::string elem{};
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      usage();
      return 0;
    } else if (elem.empty()) {
      elem = arg;
    }
  }
  return EXIT_SUCCESS;
}
