#include "GameOfLife.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

GameOfLife::GameOfLife() 
  : m_running(true) {} 


void GameOfLife::init() {
  // configure window
  m_window.setTitle("Game of Life Test for lumina-0.1");
  m_window.setVersionHint(3, 3);
  m_window.addEventCallback(bind(&GameOfLife::onEvent, this, placeholders::_1));
  m_window.resize(Vec2i(640, 480));
}

void GameOfLife::start() {
  // open the window (we need to call init before!)
  m_window.open();

  // obtain render context
  auto& renderContext = m_window.getRenderContext();
  renderContext.create();

  // we just need one context, so we can prime it here
  renderContext.prime(bind(&GameOfLife::run, this, placeholders::_1));
}

void GameOfLife::run(HotRenderContext& hotContext) {
  // create VertexSeq that represents a quad
  VertexSeq quad;
  quad.create(2, 4);  // 2 floats, 4 vertices
  quad.prime<Vec2f>([](HotVertexSeq<Vec2f>& hot){
    hot.vertex[0] = Vec2f( 1, -1);
    hot.vertex[1] = Vec2f(-1, -1);
    hot.vertex[2] = Vec2f( 1,  1);
    hot.vertex[3] = Vec2f(-1,  1);

    hot.applyVertexLayout();
  });


  // // load and compile vertex and fragment shader
  // VShader vs;
  // vs.compile(loadShaderFromFile("shader/GameOfLife.vsh"));
  // FShader fs;
  // fs.compile(loadShaderFromFile("shader/GameOfLife.fsh"));

  // // create program and link the two shaders
  // Program p;
  // p.create(vs, fs);

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    // poll events
    m_window.update();
    
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
    });

    // swap buffer
    hotContext.swapBuffer();
  }
}

lumina::EventResult GameOfLife::onEvent(InputEvent e) {
  // we just need key event
  if(e.type != InputType::Key) {
    return EventResult::Skipped;
  }

  // we just need key events when a key is pressed or hold
  auto type = e.keyInput.type;
  if(!(type == KeyInputType::Pressed || type == KeyInputType::Hold)) {
    return EventResult::Skipped;
  }

  switch(e.keyInput.key) {
    // exit application
    case KeyCode::Escape:
      m_running = false;
      break;

    // other keys don't matter
    default:
      return EventResult::Skipped;
  }

  return EventResult::Processed;
}