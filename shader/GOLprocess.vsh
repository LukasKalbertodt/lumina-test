#version 330 core

// we just get 4 vertices (+-1, +-1)
layout(location = 0) in vec2 i_pos;

uniform ivec2 u_winSize;

out VertexData {
  vec2 pos;     
  vec2 pixPos;
} o;


void main() {
  o.pos = ((i_pos + vec2(1, 1)) / 2);
  o.pixPos = o.pos * u_winSize;


  // write gl_Position -> make OpenGL happy
  gl_Position = vec4(i_pos, 0, 1);
}