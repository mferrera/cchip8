#include <cchip8/memory.h>
#include <cchip8/rom.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

namespace cchip8 {

void Memory::Reset() {
  ram.fill(0);
  vram.fill(false);
  stack.fill(0);
  std::copy(SPRITES.begin(), SPRITES.end(), ram.begin() + SPRITES_LOCATION);
}

bool Memory::LoadProgram(const Rom& rom, const size_t location) {
  Reset();
  auto data = rom.Data();
  std::copy(data.begin(), data.begin() + rom.Size(), ram.begin() + location);
  return true;
}

void Memory::DumpMem() const {
  for (auto i = 0; i < RAM_SIZE; ++i) {
    if (i % 16 == 0) {
      if (i != 0) std::cout << std::endl;
      PrintAddress(i);
    }
    PrintByte(ram.at(i));
    if (i % 2 == 1) {
      std::cout << " ";
    }
  }
  std::cout << std::endl;
}

void Memory::DumpStack() const {
  for (auto& address : stack) {
    std::cout << address << std::endl;
  }
}

void Memory::PrintByte(const uint8_t& byte) const {
  std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(byte);
}

void Memory::PrintAddress(const uint16_t& address) const {
  std::cout << std::hex << std::setw(4) << std::setfill('0')
            << static_cast<int>(address);
  std::cout << std::dec << " ";
}

}  // namespace cchip8
