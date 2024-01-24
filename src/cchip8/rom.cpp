#include <cchip8/rom.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace cchip8 {

bool Rom::FromFile(const std::string &filename) {
  if (!std::filesystem::exists(filename)) {
    std::cerr << "File does not exist." << std::endl;
    return false;
  }

  size_t fileSize = std::filesystem::file_size(filename);
  if (fileSize > MAX_ROM_SIZE) {
    std::cerr << "File size exceeds the maximum allowed size." << std::endl;
    return false;
  }

  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Could not open file." << std::endl;
    return false;
  }

  file.read(reinterpret_cast<char *>(m_data.data()), fileSize);
  if (file.bad()) {
    std::cerr << "Error reading the file." << std::endl;
    return false;
  }
  file.close();

  m_romSize = fileSize;
  m_filename = filename;
  m_loaded = true;
  return m_loaded;
}

}  // namespace cchip8
