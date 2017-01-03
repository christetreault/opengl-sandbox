#ifndef DMP_RENDERER_HPP
#define DMP_RENDERER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dmp
{
  class Renderer
  {
  public:
    //Renderer() = delete;
    Renderer(const Renderer &) = delete;
    Renderer & operator=(const Renderer &) = delete;
    Renderer(Renderer && other) = default;
    Renderer & operator=(Renderer && other) = default;

    Renderer();
    void resize(GLsizei width, GLsizei height);
  private:
    void initRenderer();

    glm::mat4 mP;
  };
};

#endif
