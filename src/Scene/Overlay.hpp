#ifndef DMP_SCENE_OVERLAY_HPP
#define DMP_SCENE_OVERLAY_HPP

#include "../Renderer/Shader.hpp"
#include "../Renderer/Texture.hpp"
#include "../Renderer/UniformBuffer.hpp"

namespace dmp
{
  struct OverlayVertex
  {
    glm::vec2 pos;
    glm::vec2 texCoords;
  };

  struct OverlayConstants
  {
    float screenWidth;
    float screenHeight;

    static size_t std140Size()
    {
      return dmp::std140PadStruct((std140ScalarSize<float>() * 2));
    }

    operator GLvoid *() {return (GLvoid *) this;}
  };

  class Overlay
  {
  public:
    Overlay() = delete;
    Overlay(const Overlay &) = delete;
    Overlay & operator=(const Overlay &) = delete;
    Overlay(Overlay &&) = default;
    Overlay & operator=(Overlay &&) = default;
    ~Overlay() {}

    void freeOverlay();
    void bind(GLenum texUnit);
    void draw();
  private:
    void initOverlay(float x,
                     float y,
                     float width,
                     float height,
                     Texture & tex);
    bool mValid = false;

    Texture mTexture;
    GLuint mVAO;
    GLuint mVBO;
    Shader mShaderProg;
  };
}
#endif
