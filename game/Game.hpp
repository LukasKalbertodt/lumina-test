#pragma once

#include "lumina/lumina.hpp"

class Game {
public:
  Game();

  void init();
  void start();

private:
  lumina::Window m_window;
  bool m_running;
  int m_steps;
  bool m_showAxis;
  bool m_antiAlias;
  float m_scale;
  lumina::Vec2f m_offset;

  void run(lumina::HotRenderContext&);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
