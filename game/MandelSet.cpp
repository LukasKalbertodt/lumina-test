#include "MandelSet.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

MandelSet::MandelSet() : m_running(true), m_steps(4), m_showAxis(true), m_antiAlias(true),
m_scale(1.6f), m_offset(-0.5f, 0.f) {} 


void MandelSet::init() {
  // configure window
  m_window.setTitle("Test MandelSet for lumina-0.1");
  m_window.setVersionHint(3, 3);
  m_window.addEventCallback(bind(&MandelSet::onEvent, this, placeholders::_1));
  m_window.resize(Vec2i(1024, 756));
}

void MandelSet::start() {
  // open the window (we need to call init before!)
  m_window.open();

  // obtain render context
  auto& renderContext = m_window.getRenderContext();
  renderContext.create();

  // we just need one context and the default FB, so we can prime them here
  renderContext.prime([this](HotRenderContext& hotContext) {
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      run(hotContext, hotFB);
    });
  });
}

void MandelSet::run(HotRenderContext& hotContext, HotFrameBuffer& hotFB) {
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

  Vec2f winSize = vector_cast<float>(m_window.getSize());

  // measure time
  auto now = chrono::system_clock::now();
  int count = 1;

  while(m_running && m_window.isValid()) {
    m_window.update();

    now = chrono::system_clock::now();

    
    p.prime([&](HotProgram& hot) {
      hot.uniform["u_winSize"] = winSize;
      hot.uniform["u_steps"] = m_steps;
      hot.uniform["u_showAxis"] = m_showAxis;
      hot.uniform["u_antiAlias"] = m_antiAlias;
      hot.uniform["u_scale"] = m_scale;
      hot.uniform["u_offset"] = m_offset;
      TexCont cont;
      cont.prime([&](HotTexCont& hotCont) {
        hot.draw(hotCont, quad, PrimitiveType::TriangleStrip);
      });
    });

    auto diff = chrono::system_clock::now() - now;
    float sec = chrono::duration_cast<chrono::milliseconds>(diff).count() / 1000.f;

    if(count++ % 50 == 0) {
      slog(" time: ", sec, " \tfps: ", 1/sec, " \t steps: ", m_steps);
    }


    hotContext.swapBuffer();
  }
}

lumina::EventResult MandelSet::onEvent(InputEvent e) {
  if(e.type == InputType::Key) {
    if(e.keyInput.type == KeyInputType::Pressed || 
       e.keyInput.type == KeyInputType::Hold) {
      if(e.keyInput.key == KeyCode::Escape) {
        m_running = false;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::Right) {
        m_steps++;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::Left) {
        m_steps--;
        if(m_steps < 1) {
          m_steps = 1;
        }
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::Space) {
        m_showAxis = !m_showAxis;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::F) {
        m_antiAlias = !m_antiAlias;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::Add) {
        m_scale *= 0.8f;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::Subtract) {
        m_scale /= 0.8f;
        return EventResult::Processed;
      }

      if(e.keyInput.key == KeyCode::W) {
        m_offset.y += 0.07 * m_scale;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::S) {
        m_offset.y -= 0.07 * m_scale;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::A) {
        m_offset.x -= 0.07 * m_scale;
        return EventResult::Processed;
      }
      if(e.keyInput.key == KeyCode::D) {
        m_offset.x += 0.07 * m_scale;
        return EventResult::Processed;
      }
    }
  }
  else if(e.type == InputType::Mouse) {

  }

  return EventResult::Skipped;
}