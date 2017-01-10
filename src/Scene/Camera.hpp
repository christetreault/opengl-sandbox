#ifndef DMP_CAMERA_HPP
#define DMP_CAMERA_HPP

#include <glm/glm.hpp>
#include "../util.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace dmp
{
  class CameraFocus
  {
  public:
    friend class Camera;

    CameraFocus(const CameraFocus &) = delete;
    CameraFocus & operator=(const CameraFocus &) = delete;

    CameraFocus(CameraFocus &&) = default;
    CameraFocus & operator=(CameraFocus &&) = default;

    void setM(glm::mat4 M)
    {
      mM = M * mBaseM;
      mDirty = true;
    }
  private:
    CameraFocus()
      : mBaseM(glm::mat4()), mM(mBaseM) {}
    CameraFocus(glm::mat4 M)
      : mBaseM(M), mM(mBaseM) {}
    glm::mat4 mBaseM;
    glm::mat4 mM;
    bool mDirty = true;
  };

  class CameraPos
  {
  public:
    friend class Camera;

    CameraPos(const CameraPos &) = delete;
    CameraPos & operator=(const CameraPos &) = delete;

    CameraPos(CameraPos &&) = default;
    CameraPos & operator=(CameraPos &&) = default;

    void setM(glm::mat4 M)
    {
      mM = M * mBaseM;
      mDirty = true;
    }
  private:
    CameraPos()
      : mBaseM(glm::mat4()), mM(mBaseM) {}
    CameraPos(glm::mat4 M)
      : mBaseM(M), mM(mBaseM) {}

    glm::mat4 mBaseM;
    glm::mat4 mM;

    bool mDirty = true;
  };

  class Camera
  {
  public:
    Camera(const Camera &) = delete;
    Camera & operator=(const Camera &) = delete;

    Camera(Camera &&) = default;
    Camera & operator=(Camera &&) = default;

    Camera()
      : mPos(), mFocus(), mUp({0.0f, 1.0f, 0.0f, 0.0f})
    {}

    Camera(glm::vec4 up)
      : mPos(), mFocus(), mUp(up)
    {}

    Camera(glm::vec4 up, glm::vec4 pos, glm::vec4 focus)
      : mPos(glm::translate(glm::mat4(),
                            glm::vec3(pos))),
        mFocus(glm::translate(glm::mat4(),
                              glm::vec3(focus))),
        mUp(up)
    {}

    CameraFocus & focus() {return mFocus;}
    CameraPos & pos() {return mPos;}

    const glm::mat4 & getV() const
    {
      expect("Get view vector", !mPos.mDirty && !mFocus.mDirty)
      return mV;
    }
    const glm::vec4 getE(glm::mat4 PV) const
    {
      expect("Get camera position", !mPos.mDirty)
      return PV * mPos.mM * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void update();
    void setUp(glm::vec4 up) {mUp = up;}
    void setPos(glm::vec4 pos)
    {
      mPos.mBaseM = glm::translate(glm::mat4(),
                                   glm::vec3(pos));
      mPos.mM = mPos.mBaseM;
      mPos.mDirty = true;
    }

    void setFocus(glm::vec4 pos)
    {
      mFocus.mBaseM = glm::translate(glm::mat4(),
                                     glm::vec3(pos));
      mFocus.mM = mFocus.mBaseM;
      mFocus.mDirty = true;
    }

  private:
    void initCamera();
    // TODO: mPos and Mfocus have poor DRY
    CameraPos mPos;
    CameraFocus mFocus;
    glm::vec4 mUp;
    glm::mat4 mV;
  };
}

#endif
