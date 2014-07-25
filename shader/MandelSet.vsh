#version 330 core


layout(location = 0) in vec2 i_pos;


out VertexData {
  vec2 pos;
} o;


void main() {
  o.pos = i_pos;
  gl_Position = vec4(i_pos, 0, 1);
}