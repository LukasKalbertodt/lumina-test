#version 330 core


in VertexData {
  vec2 pos;     
  vec2 pixPos;
} id;


uniform sampler2D tex0;

// write a single color
out vec4 o_color;

void main(){
  ivec2 pixel = ivec2(id.pixPos) / 2;

  o_color.xyz = vec3(1, 1, 1) * texelFetch(tex0, pixel, 0).x;
  o_color.w = 1.0;
}