#version 410

in vec4 normalToFrag;

out vec4 outColor;

void main()
{
  outColor = normalToFrag;
}
