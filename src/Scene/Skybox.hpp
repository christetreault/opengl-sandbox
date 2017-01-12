#ifndef DMP_SKYBOX_HPP
#define DMP_SKYBOX_HPP

#include <vector>
#include <string>
#include <GL/glew.h>
#include "../Renderer/Shader.hpp"

namespace dmp
{
  class Skybox
  {
  public:
    Skybox() = delete;
    Skybox(const Skybox &) = delete;
    Skybox & operator=(const Skybox &) = delete;
    Skybox(Skybox &&) = default;
    Skybox & operator=(Skybox &&) = default;

    ~Skybox() {}

    Skybox(std::vector<const char *> tex)
    {
      initSkybox(tex);
    }

    void freeSkybox();

    void bind(GLenum texUnit);

    void draw();

  private:
    void initSkybox(std::vector<const char *> tex);
    GLuint mTexId = 0;
    bool mValid = false;
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    Shader mShaderProg;
  };
}

#endif
