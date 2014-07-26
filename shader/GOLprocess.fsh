#version 330 core


in VertexData {
  vec2 pos;     
  vec2 pixPos;
} id;

uniform sampler2D tex0;

// write a single color
layout (location = 0) out vec4 o_color;

void main(){
  ivec2 pixel = ivec2(id.pixPos);

  // check if I am alive
  float self = texelFetch(tex0, pixel, 0).x;

  // count how many neighbours are alive
  float numLiving = texelFetch(tex0, pixel + ivec2( 0,  1), 0).x  // north
                  + texelFetch(tex0, pixel + ivec2( 1,  1), 0).x  // north east
                  + texelFetch(tex0, pixel + ivec2( 1,  0), 0).x  // east
                  + texelFetch(tex0, pixel + ivec2( 1, -1), 0).x  // south east
                  + texelFetch(tex0, pixel + ivec2( 0, -1), 0).x  // south
                  + texelFetch(tex0, pixel + ivec2(-1, -1), 0).x  // south west
                  + texelFetch(tex0, pixel + ivec2(-1,  0), 0).x  // west
                  + texelFetch(tex0, pixel + ivec2(-1,  1), 0).x; // north west

  // if I live... (GOL rules)
  bool alive = false;
  if(self == 1) {
    if(numLiving < 2) {
      alive = false;
    }
    else if(numLiving <= 3) {
      alive = true;
    }
    else {
      alive = false;
    }
  }
  else {
    if(numLiving == 3) {
      alive = true;
    }
  }

  // write 1 if we are alive, 0 otherwise
  o_color.xyz = vec3(1,1,1) * (alive ? 1 : 0);
  o_color.w = 1;
}