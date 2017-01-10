#include "Renderer.hpp"

#include <set>
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "util.hpp"
#include "config.hpp"
#include "Renderer/Pass.hpp"

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
  initPassConstants();
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
                        nearZ,
                        farZ);
}

void dmp::Renderer::initPassConstants()
{
  mPassConstants = std::make_unique<UniformBuffer>(1,
                                                   PassConstants::std140Size());
}

void dmp::Renderer::render(const Scene & scene, const Timer & timer)
{
  expect("Scene Object Constants not null",
         scene.objectConstants);

  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);

  expectNoErrors("Clear prior to render");

  size_t materialIndex = scene.objects[0].materialIndex();

  glUseProgram(mShaderProg);

  expectNoErrors("Bind shader program");

  // Pass constants

  PassConstants pc = {};

  pc.lightColor[0] = scene.lights[0].color;
  pc.lightDir[0] = scene.lights[0].M * scene.lights[0].dir;
  pc.numLights = 1;

  pc.P = mP;
  pc.invP = glm::inverse(pc.P);
  pc.V = scene.cameras[0].getV();
  pc.invP = glm::inverse(pc.V);
  pc.PV = pc.P * pc.V;
  pc.invPV = glm::inverse(pc.PV);
  pc.E = scene.cameras[0].getE(pc.PV);
  pc.nearZ = nearZ;
  pc.farZ = farZ;
  pc.deltaT = timer.deltaTime();
  pc.totalT = timer.time();

  mPassConstants->update(0, pc);

  GLuint pcIdx = glGetUniformBlockIndex(mShaderProg, "PassConstants");
  glUniformBlockBinding(mShaderProg, pcIdx, 1);
  mPassConstants->bind(1, 0);

  // Material Constants

  GLuint mcIdx = glGetUniformBlockIndex(mShaderProg, "MaterialConstants");
  glUniformBlockBinding(mShaderProg, mcIdx, 2);
  scene.materialConstants->bind(2, materialIndex);

  // Object Constants

  GLuint ocIdx = glGetUniformBlockIndex(mShaderProg, "ObjectConstants");
  glUniformBlockBinding(mShaderProg, ocIdx, 3);

  for (size_t i = 0; i < scene.objects.size(); ++i)
    {
      if (scene.objects[i].materialIndex() != materialIndex)
        {
          materialIndex = scene.objects[i].materialIndex();
          scene.materialConstants->bind(2, materialIndex);
        }

       if (scene.objects[i].isTextured())
         {
           glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D,
                         scene.textures[scene.objects[i].textureIndex()]);
           glUniform1i(glGetUniformLocation(mShaderProg, "tex"), 0);
         }

      scene.objectConstants->bind(3, i);

      expectNoErrors("Set uniforms");

      scene.objects[i].bind();
      scene.objects[i].draw();
    }
}
