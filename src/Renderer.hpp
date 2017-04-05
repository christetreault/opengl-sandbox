#ifndef DMP_RENDERER_HPP
#define DMP_RENDERER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Scene.hpp"
#include "Renderer/Shader.hpp"
#include "Timer.hpp"

namespace dmp
{
  struct RenderOptions
  {
    bool drawWireframe = false;
    bool drawNormals = false;
    bool drawOverlays = false;
  };

  class Renderer
  {
  public:
    Renderer() = delete;
    Renderer(const Renderer &) = delete;
    Renderer & operator=(const Renderer &) = delete;
    Renderer(Renderer && other) = default;
    Renderer & operator=(Renderer && other) = default;
    ~Renderer() = default;

    Renderer(GLsizei width, GLsizei height);
    void resize(GLsizei width, GLsizei height);

    void render(const Scene & scene,
                const Timer & timer,
                const RenderOptions & ro);

    int pick(const Scene & scene, const RenderOptions & ro, int x, int y);
  private:
    void initRenderer();
    void loadShaders(const std::string shaderFile,
                     Shader & shaderProg);
    void initPassConstants();

    glm::mat4 mP;
    Shader mShaderProg;
    Shader mOverlayShaderProg;
    Shader mOverlayPickingShaderProg;

    std::unique_ptr<UniformBuffer> mPassConstants;

    GLsizei mWidth;
    GLsizei mHeight;
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
