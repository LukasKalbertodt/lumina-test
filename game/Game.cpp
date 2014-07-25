#include "Game.hpp"

#include <functional>

using namespace lumina;
using namespace std;

Game::Game() : m_running(true) {}

void Game::init() {
  // configure window
  m_window.setTitle("Test Game for lumina-0.1");
  m_window.setVersionHint(3, 3);
  m_window.addEventCallback(bind(&Game::onEvent, this, placeholders::_1));
}

void Game::start() {
  m_window.open();

  auto& renderContext = m_window.getRenderContext();
  renderContext.create();
  renderContext.prime(bind(&Game::run, this, placeholders::_1));
}

void Game::run(HotRenderContext& hotContext) {
  VertexSeq quad;
  quad.create(2, 4);
  quad.prime<Vec2f>([](HotVertexSeq<Vec2f>& hot){
    hot.vertex[0] = Vec2f( 1, -1);
    hot.vertex[1] = Vec2f(-1, -1);
    hot.vertex[2] = Vec2f( 1,  1);
    hot.vertex[3] = Vec2f(-1,  1);

    hot.applyVertexLayout();
  });

  VShader vs;
  vs.compile(loadShaderFromFile("shader/MandelSet.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/MandelSet.fsh"));

  Program p;
  p.create(vs, fs);

  while(m_running && m_window.isValid()) {
    m_window.update();

    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      p.prime([&](HotProgram& hot) {
        TexCont cont;
        cont.prime([&](HotTexCont& hotCont) {
          hot.draw(hotCont, quad, PrimitiveType::TriangleStrip);
        });
      });
    });

    hotContext.swapBuffer();
  }
}

lumina::EventResult Game::onEvent(InputEvent e) {
  if(e.type == InputType::Key) {
    if(e.keyInput.type == KeyInputType::Pressed 
       && e.keyInput.key == KeyCode::Escape) {
      m_running = false;
      return EventResult::Processed;
    }
  }
  else if(e.type == InputType::Mouse) {

  }

  return EventResult::Skipped;
}