#version 330 core

// linear interpolated: pos is exactly the complex number we need to draw
in VertexData {
  vec2 pos;
} id;

// a bunch of uniforms
uniform vec2 u_winSize;
uniform int u_steps;
uniform bool u_showAxis;
uniform bool u_antiAlias;
uniform float u_scale;

// write a single color
out vec4 o_color;


// calculates the number of steps it takes that cdot leaves the set
int isInSet(vec2 cdot, int steps) {
  // c will be iterated
  vec2 c = cdot;

  // make *steps* maximum steps
  for(int i = 0; i < steps; i++) {
    // calculate x-squared and y-squared
    float xs = c.x*c.x;
    float ys = c.y*c.y;

    // iterate c_n = (c_(n-1))^2 + cdot
    c = vec2(xs - ys, 2*c.y*c.x) + cdot;

    // if the magnitute^2 is larger than 4, it has left the set 
    if(xs + ys > 4) {
      return i;
    }
  }

  // number didn't left the set with *steps* steps...
  return steps;
}


void main(){
  // show axis 
  if(u_showAxis) {
    // cast to int -> color just one pixel
    if(int((id.pos.x / u_scale) * 0.5 * u_winSize.y + 1) == 1 || 
       int((id.pos.y / u_scale) * 0.5 * u_winSize.y + 1) == 1) {
      o_color = vec4(0,0.4,1,1);
      return;
    }
  }

  // calculate strength
  float strength = 0;
  if(!u_antiAlias) {
    // without anti aliasing, just solve one time
    strength = isInSet(id.pos, u_steps) / float(u_steps);
  }
  else {
    // anti aliasing: sample 4 points around the original dot +- delta
    // d = delta = 1/4 pixel
    float d = (0.5 * u_scale) / u_winSize.x;
    strength += isInSet(id.pos + vec2(-d, -d), u_steps) / (u_steps * 4.0);
    strength += isInSet(id.pos + vec2( d, -d), u_steps) / (u_steps * 4.0);
    strength += isInSet(id.pos + vec2(-d,  d), u_steps) / (u_steps * 4.0);
    strength += isInSet(id.pos + vec2( d,  d), u_steps) / (u_steps * 4.0);
  }

  // set a grey color, white if the fragment is in the set
  o_color.xyz = vec3(1,1,1) * strength;
  o_color.w = 1.0;
}