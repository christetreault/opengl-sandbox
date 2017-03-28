#version 410

layout (location = 0) in vec3 posToVert;
layout (location = 1) in vec3 normalToVert;
layout (location = 2) in vec2 texCoordToVert;

layout (std140) uniform PassConstants
{
  vec4 lightColor[8]; // maxLights = 8
  vec4 lightDir[8];
  uint numLights;
  uint drawMode;

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
};

layout (std140) uniform ObjectConstants
{
  mat4 M;
  mat4 normalM;
};

out vec3 normalToFrag;
out vec3 posToFrag;
out vec2 texCoordToFrag;

void main()
{
  gl_Position = PV * M * vec4(posToVert, 1.0f);
  normalToFrag = vec3(normalize(normalM * vec4(normalToVert, 0.0f)));
  posToFrag = vec3(M * vec4(posToVert, 1.0f));
  texCoordToFrag = texCoordToVert;
}
