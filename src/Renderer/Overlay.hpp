#ifndef DMP_SCENE_OVERLAY_HPP
#define DMP_SCENE_OVERLAY_HPP

#include <cstdint>
#include "Shader.hpp"
#include "Texture.hpp"
#include "UniformBuffer.hpp"

namespace dmp
{
  struct OverlayVertex
  {
    glm::vec2 pos;
    glm::vec2 texCoords;
  };

  struct OverlayConstants
  {
    int overlayID;

    static size_t std140Size()
    {
      return dmp::std140PadStruct((std140ScalarSize<int>()));
    }

    operator GLvoid *() {return (GLvoid *) this;}
  };

  class Overlay
  {

    static const size_t drawCount = 6;
  public:
    Overlay() = delete;
    Overlay(const Overlay &) = delete;
    Overlay & operator=(const Overlay &) = delete;
    Overlay(Overlay &&) = default;
    Overlay & operator=(Overlay &&) = default;
    ~Overlay() {}

    Overlay(float x,
            float y,
            float width,
            float height,
            int id,
            Texture & tex);

    Overlay(float x,
            float y,
            float width,
            float height,
            Texture & tex);

    void freeOverlay();
    void bind(GLenum texUnit);
    void draw();
    OverlayConstants getOverlayConstants() const;
    void draw() const;
    bool isDirty() const {return mDirty;}
    void setClean() {mDirty = false;}
    GLuint getTexture() const {return mTexture;}
  private:
    void initOverlay(float x,
                     float y,
                     float width,
                     float height,
                     int aspectRatio,
                     Texture & tex);
    bool mValid = false;
    bool mDirty = true;

    Texture mTexture;
    int mID = 255;
    bool mVisible = true;
    GLuint mVAO;
    GLuint mVBO;
  };
}
#endif
