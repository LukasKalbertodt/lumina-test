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

  void run(lumina::HotRenderContext&);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
