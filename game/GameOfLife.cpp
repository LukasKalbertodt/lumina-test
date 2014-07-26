#include "GameOfLife.hpp"

#include <functional>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

using namespace lumina;
using namespace std;

GameOfLife::GameOfLife() 
  : m_running(true) {} 


void GameOfLife::init() {
  // FrameBuffer would spam a lot
  Logger::setGlobalStdLevelFilter(LogLevel::Info);

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


  // load and compile vertex and fragment shader
  VShader vsSimple;
  vsSimple.compile(loadShaderFromFile("shader/GOLsimple.vsh"));
  FShader fsSimple;
  fsSimple.compile(loadShaderFromFile("shader/GOLsimple.fsh"));

  // create program and link the two shaders
  Program pSimple;
  pSimple.create(vsSimple, fsSimple);



  VShader vsProcess;
  vsProcess.compile(loadShaderFromFile("shader/GOLprocess.vsh"));
  FShader fsProcess;
  fsProcess.compile(loadShaderFromFile("shader/GOLprocess.fsh"));

  // create program and link the two shaders
  Program pProcess;
  pProcess.create(vsProcess, fsProcess);


  // create texture with random start world
  vector<uint8_t> startWorld((m_window.getSize() / 2).linearSize() * 3);
  random_device randomDev;
  mt19937 rng(randomDev());
  uniform_int_distribution<int> dist(0, 7);
  for(auto& field : startWorld) {
    field = (dist(rng) != 0) ? 0 : 255;
  }

  Tex2D worldTexA, worldTexB;
  worldTexA.params.wrapMode = TexWrapMode::Repeat;
  worldTexA.create(m_window.getSize() / 2, TexFormat::RGB8, startWorld.data());
  worldTexB.params.wrapMode = TexWrapMode::Repeat;
  worldTexB.create(m_window.getSize() / 2, TexFormat::RGB8);

  Tex2D* refA = &worldTexA;
  Tex2D* refB = &worldTexB;




  FrameBuffer procFB;
  procFB.create();


  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    // poll events
    m_window.update();

    TexCont contA;
    contA.addTexture(0, *refA);
    TexCont contB;
    contB.addTexture(0, *refB);


    
    procFB.attachColor(0, *refB);
    procFB.prime([&](HotFrameBuffer& hotFB) {
    // hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      pProcess.prime([&](HotProgram& hotProg) {
        hotProg.uniform["u_winSize"] = m_window.getSize();

        contA.prime([&](HotTexCont& hotCont) {
          hotProg.draw(hotCont, quad, PrimitiveType::TriangleStrip);
        });
      });

    });

    

    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      pSimple.prime([&](HotProgram& hotProg) {
        hotProg.uniform["u_winSize"] = m_window.getSize();
        contB.prime([&](HotTexCont& hotCont) {
          hotProg.draw(hotCont, quad, PrimitiveType::TriangleStrip);
        });
      });
    });

    // swap textures
    Tex2D* tmp = refA;
    refA = refB;
    refB = tmp;

    this_thread::sleep_for(chrono::milliseconds(50));

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