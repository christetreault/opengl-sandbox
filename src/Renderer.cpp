#include "Renderer.hpp"

#include <set>
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "util.hpp"
#include "config.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

dmp::Renderer::Renderer(GLsizei width,
                        GLsizei height,
                        const std::string shaderName)
{
  initRenderer();
  loadShaders(shaderName);
  resize(width, height);
}

void dmp::Renderer::loadShaders(const std::string shaderName)
{
  auto vertName = shaderName + std::string(".vert");
  auto fragName = shaderName + std::string(".frag");

  mShaderProg.initShader(vertName.c_str(),
                         nullptr, nullptr, nullptr,
                         fragName.c_str());
}

void dmp::Renderer::initRenderer()
{
  expect("Init Glew",
         glewInit() == GLEW_OK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_PROGRAM_POINT_SIZE);

  expectNoErrors();
}

void dmp::Renderer::resize(GLsizei width, GLsizei height)
{
  float fWidth = (float) width;
  float fHeight = (float) height;
  glViewport(0, 0, width, height);

  mP = glm::perspective(fieldOfView,
                        fWidth / fHeight,
                        0.1f,
                        1000.0f);
}

void dmp::Renderer::render(const Scene & scene)
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  expectNoErrors("Clear prior to render");

  auto M = scene.objects[0].getM();
  auto V = scene.cameras[0].V;
  auto P = mP;

  auto PVM = P * V * M;

  glUseProgram(mShaderProg);

  expectNoErrors("Bind shader program");

  glUniformMatrix4fv(glGetUniformLocation(mShaderProg, "PVM"),
                     1,
                     GL_FALSE,
                     &PVM[0][0]);

  expectNoErrors("Set uniforms");

  scene.objects[0].bind();
  scene.objects[0].draw();
}