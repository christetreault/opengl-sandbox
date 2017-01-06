#ifndef DMP_RENDERER_HPP
#define DMP_RENDERER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Scene.hpp"
#include "Renderer/Shader.hpp"

namespace dmp
{
  class Renderer
  {
  public:
    Renderer() = delete;
    Renderer(const Renderer &) = delete;
    Renderer & operator=(const Renderer &) = delete;
    Renderer(Renderer && other) = default;
    Renderer & operator=(Renderer && other) = default;

    Renderer(GLsizei width, GLsizei height, const std::string shaderFile);
    void resize(GLsizei width, GLsizei height);

    void render(const Scene & scene);
  private:
    void initRenderer();
    void loadShaders(const std::string shaderFile);

    glm::mat4 mP;
    Shader mShaderProg;
  };

  // Opengl constants

  inline int uniformBufferOffsetAlignment()
  {
    int alignment;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
    return alignment;
  }
};

#endif
