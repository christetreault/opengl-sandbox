#include "Renderer.hpp"

#include <set>
#include <algorithm>
#include <iostream>
#include "util.hpp"
#include "config.hpp"

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
  glViewport(0, 0, width, height);

  mP = glm::
}
