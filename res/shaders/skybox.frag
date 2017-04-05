#version 410 core

uniform samplerCube skybox;

layout (std140) uniform PassConstants
{
  vec4 lightColor[8]; // maxLights = 8
  vec4 lightDir[8];

  uint numLights;

  mat4 P;
  mat4 invP;
  mat4 V;
  mat4 invV;
  mat4 PV;
  mat4 invPV;

  vec4 E;

  float nearZ;
  float farZ;
  float deltaT;
  float totalT;

  float viewportWidth;
  float viewportHeight;
};

in vec3 texCoordsToFrag;
out vec4 color;

void main()
{
  color = texture(skybox, texCoordsToFrag);
}
