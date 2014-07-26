#version 330 core

in VertexData {
  vec2 pos;
} id;

uniform vec2 u_winSize;
uniform int u_steps;
uniform bool u_showAxis;
uniform bool u_antiAlias;
uniform float u_scale;

const float EPSILON = 0.003;


layout (location = 0) out vec4 o_color;

int isInSet(vec2 cdot, int steps) {
  vec2 c = cdot;
  for(int i = 0; i < steps; i++) {
    float xs = c.x*c.x;
    float ys = c.y*c.y;
    c = vec2(xs - ys, 2*c.y*c.x) + cdot;
    if(xs + ys > 4) {
      return i;
    }
  }
  return steps;
}
 
void main(){
  if(u_showAxis) {
    if(int((id.pos.x / u_scale) * 0.5 * u_winSize.y + 1) == 1 || 
       int((id.pos.y / u_scale) * 0.5 * u_winSize.y + 1) == 1) {
      o_color = vec4(0,0.4,1,1);
      return;
    }
  }

  float strength = 0;
  if(!u_antiAlias) {
    strength = isInSet(id.pos, u_steps) / float(u_steps);
  }
  else {
    float dx = (0.5 * u_scale) / u_winSize.x;
    float dy = (0.5 * u_scale) / u_winSize.y;
    strength += isInSet(id.pos + vec2(-dx, -dy), u_steps) / (u_steps * 4.0);
    strength += isInSet(id.pos + vec2( dx, -dy), u_steps) / (u_steps * 4.0);
    strength += isInSet(id.pos + vec2(-dx,  dy), u_steps) / (u_steps * 4.0);
    strength += isInSet(id.pos + vec2( dx,  dy), u_steps) / (u_steps * 4.0);
  }
  o_color.xyz = vec3(1,1,1) * strength;
  o_color.w = 1.0;
}