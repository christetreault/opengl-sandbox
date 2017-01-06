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
  ifDebug(std::cerr
          << "Supported GLSL version: "
          << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION)
          << std::endl);
  ifDebug(std::cerr
          << "Supported GLSL version: "
          << (char *)glGetString(GL_VERSION)
          << std::endl);
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
  // This whole section is quite icky. Maybe I shouldn't be using glew...
  glewExperimental = GL_TRUE;
  expectNoErrors("begin initrenderer");
  expect("Init Glew",
         glewInit() == GLEW_OK);

  auto err = glGetError();

  expect("glewInit will report GL_INVALID_ENUM",
         err == GL_INVALID_ENUM || err == GL_NO_ERROR);

  expectNoErrors("init glew");
  // end yuckiness

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_PROGRAM_POINT_SIZE);

  expectNoErrors("init renderer");
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
  expect("Scene Object Constants not null",
         scene.objectConstants);

  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  expectNoErrors("Clear prior to render");

  auto V = scene.cameras[0].V;
  auto P = mP;

  auto PV = P * V;

  glUseProgram(mShaderProg);

  expectNoErrors("Bind shader program");

  glUniformMatrix4fv(glGetUniformLocation(mShaderProg, "PV"),
                     1,
                     GL_FALSE,
                     &PV[0][0]);

  GLuint ocIdx = glGetUniformBlockIndex(mShaderProg, "ObjectConstants");
  glUniformBlockBinding(mShaderProg, ocIdx, 3);

  for (size_t i = 0; i < scene.objects.size(); ++i)
    {
      scene.objectConstants->bind(3, i);

      expectNoErrors("Set uniforms");

      scene.objects[i].bind();
      scene.objects[i].draw();
    }
}
