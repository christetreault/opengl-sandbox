#version 410 core

layout (location = 0) in vec3 positionToVert;

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
};

out vec3 texCoordsToFrag;

void main()
{
  gl_Position = P * mat4(mat3(V)) * vec4(positionToVert,
                                         1.0);
  texCoordsToFrag = positionToVert;
}
