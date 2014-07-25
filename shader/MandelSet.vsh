#version 330 core


layout(location = 0) in vec2 i_pos;

uniform float u_scale;
uniform vec2 u_offset;
uniform vec2 u_winSize;

out VertexData {
  vec2 pos;
} o;


void main() {
  o.pos = i_pos * u_scale + u_offset;
  o.pos.x /= (u_winSize.y / u_winSize.x);
  gl_Position = vec4(i_pos, 0, 1);
}