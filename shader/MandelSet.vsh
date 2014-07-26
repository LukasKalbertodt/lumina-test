#version 330 core

// we just get 4 vertices (+-1, +-1)
layout(location = 0) in vec2 i_pos;

// a bunch of uniforms
uniform float u_scale;
uniform vec2 u_offset;
uniform vec2 u_winSize;

// output the plane we want to look at
out VertexData {
  vec2 pos;
} o;


void main() {
  // scale and move plane 
  o.pos = i_pos * u_scale + u_offset;

  // make sure we have the right aspect ratio
  o.pos.x /= (u_winSize.y / u_winSize.x);

  // write gl_Position -> make OpenGL happy
  gl_Position = vec4(i_pos, 0, 1);
}