#version 410

layout (location = 0) in vec4 posToVert;
layout (location = 1) in vec4 normalToVert;

uniform mat4 PV;

layout (std140) uniform ObjectConstants
{
  mat4 M;
};

out vec4 normalToFrag;

void main()
{
  gl_Position = PV * M * posToVert;
  normalToFrag = normalToVert;
}
