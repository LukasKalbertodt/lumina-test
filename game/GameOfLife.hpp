#pragma once

#include "lumina/lumina.hpp"

class GameOfLife {
public:
  GameOfLife();

  void init();
  void start();

private:
  lumina::Window m_window;
  bool m_running;

  void run(lumina::HotRenderContext&);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
