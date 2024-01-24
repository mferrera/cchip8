#include <cchip8/emulator.h>
#include <cchip8/memory.h>
#include <cchip8/rom.h>

#include <iostream>
#include <string>

void usage() { std::cout << "Usage: cchip8 rom.ch8" << std::endl; }

int main(int argc, char** argv) {
  if (argc < 2) {
    usage();
    return EXIT_SUCCESS;
  }

  std::string file{};
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      usage();
      return EXIT_SUCCESS;
    } else if (file.empty()) {
      file = arg;
    }
  }

  cchip8::Rom rom;
  if (!rom.FromFile(file)) {
    return EXIT_FAILURE;
  }

  cchip8::Emulator emulator;
  if (emulator.LoadRom(rom)) {
    emulator.Start();
  } else {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
