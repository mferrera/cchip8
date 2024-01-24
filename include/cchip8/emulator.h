#ifndef CCHIP8_EMULATOR_H_
#define CCHIP8_EMULATOR_H_

#include <cchip8/audio.h>
#include <cchip8/cpu.h>
#include <cchip8/display.h>
#include <cchip8/input.h>
#include <cchip8/instruction.h>
#include <cchip8/memory.h>
#include <cchip8/rom.h>

#define TICKS_PER_FRAME 10

namespace cchip8 {

class Emulator {
 public:
  ~Emulator();

  bool LoadRom(const Rom& rom);
  [[nodiscard]] bool RomLoaded() { return m_rom_loaded; };
  void Start();
  void Reset();

 private:
  bool m_running{false};
  bool m_rom_loaded{false};
  bool m_draw{false};

  Rom m_rom{};
  Cpu m_cpu{};
  Memory m_memory{};
  Input m_input{};
  Display m_display{};
  Audio m_audio{};
  SDL_Event m_event{};

  bool InitDevices();
  void MainLoop();
  void UpdateTimers();
  void ToggleSound();
  void HandleEvent(const SDL_Event& event);
  void PollEvents();

  Instruction Fetch();
  void Update();
  void Draw();
  void Tick();
};

}  // namespace cchip8

#endif  // CCHIP8_EMULATOR_H_
