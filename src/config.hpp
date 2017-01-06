#ifndef DMP_CONFIG_HPP
#define DMP_CONFIG_HPP

#include <vector>
#include <map>
#include <glm/glm.hpp>


namespace dmp
{
  static const float fieldOfView = glm::radians(60.0f);

  // some random test model for the test scene implementation
  static const char * testModel = "res/models/model.ply";

  static const float nearZ = 0.1f;
  static const float farZ = 1000.0f;

  static const size_t maxLights = 8;
}

#endif
