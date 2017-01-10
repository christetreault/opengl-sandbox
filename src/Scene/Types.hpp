#ifndef DMP_TYPES_HPP
#define DMP_TYPES_HPP

#include <glm/glm.hpp>
#include "../Renderer/UniformBuffer.hpp"

namespace dmp
{
  struct Material
  {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;

    static size_t std140Size()
    {
      return dmp::std140PadStruct((std140VecSize<float, 4>() * 3)
                                  + std140ScalarSize<float>());
    }

    operator GLvoid *() {return (GLvoid *) this;}
  };

  struct Light
  {
    glm::vec4 color;
    glm::vec4 dir;
    glm::mat4 M;
  };
}

#endif
