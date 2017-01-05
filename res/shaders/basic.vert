#version 410

layout (location = 0) in vec4 posToVert;
layout (location = 1) in vec4 normalToVert;

uniform mat4 PVM;

out vec4 normalToFrag;

void main()
{
  gl_Position = PVM * posToVert;
  normalToFrag = normalToVert;
}
