#ifndef DMP_CONFIG_HPP
#define DMP_CONFIG_HPP

#include <vector>
#include <map>
#include <glm/glm.hpp>


namespace dmp
{
  static float fieldOfView = glm::radians(60.0f);

  // some random test model for the test scene implementation
  static const char * testModel = "res/models/model.ply";
}

#endif
