#include <SDL.h>
#include <cchip8/cpu.h>
#include <cchip8/emulator.h>
#include <cchip8/input.h>
#include <cchip8/memory.h>
#include <cchip8/rom.h>
#include <cchip8/window.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace cchip8 {

bool Emulator::LoadRom(const Rom& rom) {
  if (m_running) {
    std::cerr << "Cannot load rom, emulator is already running one."
              << std::endl;
    return false;
  }
  m_rom = rom;
  m_rom_loaded = m_memory.LoadProgram(m_rom, PROGRAM_START);
  m_cpu.pc = PROGRAM_START;
  return m_rom_loaded;
}

void Emulator::Reset() {
  m_window.Clear();
  m_cpu.Reset();
  m_memory.Reset();
  m_input.Reset();
  m_audio.Reset();
}

bool Emulator::InitDevices() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }
  auto initDisplay = m_window.Init();
  auto initAudio = m_audio.Init();
  return initDisplay && initAudio;
}

void Emulator::Start() {
  if (!m_rom_loaded) {
    std::cerr << "Cannot start emulator, no ROM has been loaded." << std::endl;
    return;
  }
  if (InitDevices()) {
    m_running = true;
    MainLoop();
  }
}

void Emulator::MainLoop() {
  using namespace std::chrono;
  auto lastDrawTime = high_resolution_clock::now();

  while (m_running && m_window.running) {
    auto currentTime = high_resolution_clock::now();
    auto elapsedTime = duration_cast<milliseconds>(currentTime - lastDrawTime);

    if (elapsedTime.count() > (1000.0 / 60.0)) {
      lastDrawTime = currentTime;
      Update();
    }
    std::this_thread::sleep_for(milliseconds(16));
  }
}

void Emulator::UpdateTimers() {
  if (m_cpu.t_delay > 0) {
    --m_cpu.t_delay;
  }
  if (m_cpu.t_sound > 0) {
    --m_cpu.t_sound;
  }
}

void Emulator::UpdateSound() {
  if (m_cpu.t_sound > 0) {
    m_audio.StartBeep();
  } else {
    m_audio.StopBeep();
  }
}

void Emulator::HandleEvent(const SDL_Event& event) {
  switch (event.type) {
    case SDL_EVENT_QUIT:
      m_running = false;
      break;
  }
}

void Emulator::PollEvents() {
  while (SDL_PollEvent(&m_event) != 0) {
    m_input.HandleEvent(m_event);
    m_window.HandleEvent(m_event);
    HandleEvent(m_event);
  }
}

void Emulator::Update() {
  for (auto tick = 0; tick < TICKS_PER_FRAME / 2; ++tick) {
    Tick();
  }
  UpdateTimers();
  UpdateSound();
  PollEvents();
  for (auto tick = 0; tick < TICKS_PER_FRAME / 2; ++tick) {
    Tick();
  }
  if (m_draw) {
    m_window.Draw(m_memory);
    m_draw = false;
  }
}

Instruction Emulator::Fetch() {
  uint16_t instruction =
      (m_memory.ram.at(m_cpu.pc) << 8) | m_memory.ram.at(m_cpu.pc + 1);
  m_cpu.pc += 2;
  return Instruction(instruction);
}

void Emulator::Tick() {
  auto instruction = Fetch();
  switch (instruction.Decode()) {
    case Opcode::CLS:
      return m_cpu.CLS(m_memory);
    case Opcode::RET:
      return m_cpu.RET(m_memory);
    case Opcode::SYS:
      return m_cpu.SYS();
    case Opcode::JP:
      return m_cpu.JP(instruction);
    case Opcode::CALL:
      return m_cpu.CALL(instruction, m_memory);
    case Opcode::SE_VX_KK:
      return m_cpu.SE_VX_KK(instruction);
    case Opcode::SNE_VX_KK:
      return m_cpu.SNE_VX_KK(instruction);
    case Opcode::SE_VX_VY:
      return m_cpu.SE_VX_VY(instruction);
    case Opcode::LD_VX_KK:
      return m_cpu.LD_VX_KK(instruction);
    case Opcode::ADD_VX_KK:
      return m_cpu.ADD_VX_KK(instruction);
    case Opcode::LD_VX_VY:
      return m_cpu.LD_VX_VY(instruction);
    case Opcode::OR_VX_VY:
      return m_cpu.OR_VX_VY(instruction);
    case Opcode::AND_VX_VY:
      return m_cpu.AND_VX_VY(instruction);
    case Opcode::XOR_VX_VY:
      return m_cpu.XOR_VX_VY(instruction);
    case Opcode::ADD_VX_VY:
      return m_cpu.ADD_VX_VY(instruction);
    case Opcode::SUB_VX_VY:
      return m_cpu.SUB_VX_VY(instruction);
    case Opcode::SHR_VX:
      return m_cpu.SHR_VX(instruction);
    case Opcode::SUBN_VX_VY:
      return m_cpu.SUBN_VX_VY(instruction);
    case Opcode::SHL_VX:
      return m_cpu.SHL_VX(instruction);
    case Opcode::SNE_VX_VY:
      return m_cpu.SNE_VX_VY(instruction);
    case Opcode::LD_I:
      return m_cpu.LD_I(instruction);
    case Opcode::JP_V0:
      return m_cpu.JP_V0(instruction);
    case Opcode::RND_VX_KK:
      return m_cpu.RND_VX_KK(instruction);
    case Opcode::DRW_VX_VY:
      m_draw = true;
      return m_cpu.DRW_VX_VY(instruction, m_memory);
    case Opcode::SKP_VX:
      return m_cpu.SKP_VX(instruction, m_input);
    case Opcode::SKNP_VX:
      return m_cpu.SKNP_VX(instruction, m_input);
    case Opcode::LD_VX_DT:
      return m_cpu.LD_VX_DT(instruction);
    case Opcode::LD_VX_K:
      return m_cpu.LD_VX_K(instruction, m_input);
    case Opcode::LD_DT_VX:
      return m_cpu.LD_DT_VX(instruction);
    case Opcode::LD_ST_VX:
      return m_cpu.LD_ST_VX(instruction);
    case Opcode::ADD_I_VX:
      return m_cpu.ADD_I_VX(instruction);
    case Opcode::LD_F_VX:
      return m_cpu.LD_F_VX(instruction);
    case Opcode::LD_B_VX:
      return m_cpu.LD_B_VX(instruction, m_memory);
    case Opcode::LD_I_VX:
      return m_cpu.LD_I_VX(instruction, m_memory);
    case Opcode::LD_VX_I:
      return m_cpu.LD_VX_I(instruction, m_memory);
    default:
      return;  // Consider unknowns as NOPs
  }
}

Emulator::~Emulator() {
  m_window.Quit();
  m_audio.Quit();
  SDL_Quit();
}

}  // namespace cchip8
