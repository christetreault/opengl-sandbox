#ifndef DMP_TYPES_HPP
#define DMP_TYPES_HPP

#include <glm/glm.hpp>

namespace dmp
{
  struct Material
  {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
  };

  struct Light
  {
    glm::vec4 color;
    glm::vec4 dir;
  };

  struct Camera
  {
    glm::vec4 pos;
    glm::vec4 up;
    glm::vec4 focus;
    glm::mat4 V;
  };
}

#endif
