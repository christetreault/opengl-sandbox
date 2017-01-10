#include "Camera.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

void dmp::Camera::update()
{
  if (mPos.mDirty || mFocus.mDirty)
    {
      mV = glm::lookAt(glm::vec3(mPos.mM * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
                       glm::vec3(mFocus.mM * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
                       glm::vec3(mUp));

      mPos.mDirty = false;
      mFocus.mDirty = false;
    }
}
