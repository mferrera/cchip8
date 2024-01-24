#ifndef CCHIP8_ROM_H_
#define CCHIP8_ROM_H_

#include <array>
#include <cstdint>
#include <string>

#define MAX_ROM_SIZE 3584  // 4096 - 512

namespace cchip8 {

class Rom {
 public:
  uint8_t operator[](size_t index) const { return m_data.at(index); }
  uint8_t& operator[](size_t index) { return m_data.at(index); }

  [[nodiscard]] bool FromFile(const std::string& filename);

  const std::array<uint8_t, MAX_ROM_SIZE>& Data() const { return m_data; }
  const std::string& Filename() const { return m_filename; };
  size_t Size() const { return m_romSize; };
  [[nodiscard]] bool Loaded() const { return m_loaded; };

 private:
  std::array<uint8_t, MAX_ROM_SIZE> m_data{};
  std::string m_filename{};
  size_t m_romSize{};
  bool m_loaded{};
};

}  // namespace cchip8

#endif  // CCHIP8_ROM_H
