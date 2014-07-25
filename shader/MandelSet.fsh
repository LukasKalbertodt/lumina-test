#version 330 core

in VertexData {
  vec2 pos;
} i;



layout (location = 0) out vec4 o_color;
 
void main(){
  o_color.xyz = vec3(1, 1, 1) * (i.pos.x + i.pos.y + 2) * 0.25;
  o_color.w = 1.0;
}