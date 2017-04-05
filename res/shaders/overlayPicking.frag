#version 410

out vec4 outColor;

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

  float viewportWidth;
  float viewportHeight;
};

layout (std140) uniform OverlayConstants
{
  int overlayID;
};

uniform sampler2D tex;

void main()
{
  outColor = vec4(overlayID / 255.0f, 0.0f, 0.0f, 1.0f);
}
