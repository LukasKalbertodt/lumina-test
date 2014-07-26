#include "MandelSet.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

MandelSet::MandelSet() 
  : m_running(true), 
    m_steps(4), 
    m_showAxis(true), 
    m_antiAlias(true),
    m_scale(1.6f), 
    m_offset(-0.5f, 0.f) {} 


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


  // load and compile vertex and fragment shader
  VShader vs;
  vs.compile(loadShaderFromFile("shader/MandelSet.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/MandelSet.fsh"));

  // create program and link the two shaders
  Program p;
  p.create(vs, fs);


  // save window size as float -> need that for a uniform variable
  Vec2f winSize = vector_cast<float>(m_window.getSize());


  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    // poll events
    m_window.update();
    
    // prime program to use it
    p.prime([&](HotProgram& hot) {
      // set all needed uniform variables
      hot.uniform["u_winSize"] = winSize;
      hot.uniform["u_steps"] = m_steps;
      hot.uniform["u_showAxis"] = m_showAxis;
      hot.uniform["u_antiAlias"] = m_antiAlias;
      hot.uniform["u_scale"] = m_scale;
      hot.uniform["u_offset"] = m_offset;

      // draw the quad
      hot.draw(quad, PrimitiveType::TriangleStrip);
    });

    // swap buffer
    hotContext.swapBuffer();
  }
}

lumina::EventResult MandelSet::onEvent(InputEvent e) {
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

    // increase or decrease number of steps
    case KeyCode::Right:
      m_steps++;
      slog(" steps: ", m_steps);
      break;
    case KeyCode::Left:
      m_steps--;
      if(m_steps < 1) {
        m_steps = 1;
      }
      slog(" steps: ", m_steps);
      break;

    // toggle axis or anti aliasing
    case KeyCode::Space:
      m_showAxis = !m_showAxis;
      break;
    case KeyCode::F:
      m_antiAlias = !m_antiAlias;
      break;

    // zoom in or out
    case KeyCode::Add:
      m_scale *= 0.8f;
      break;
    case KeyCode::Subtract:
      m_scale /= 0.8f;
      break;

    // move
    case KeyCode::W:
      m_offset.y += 0.07 * m_scale;
      break;
    case KeyCode::S:
      m_offset.y -= 0.07 * m_scale;
      break;
    case KeyCode::A:
      m_offset.x -= 0.07 * m_scale;
      break;
    case KeyCode::D:
      m_offset.x += 0.07 * m_scale;
      break;

    // other keys don't matter
    default:
      return EventResult::Skipped;
  }

  return EventResult::Processed;
}