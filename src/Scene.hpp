#ifndef DMP_SCENE_HPP
#define DMP_SCENE_HPP

#include "Scene/Types.hpp"
#include "Scene/Object.hpp"
#include "Scene/Graph.hpp"

namespace dmp
{
  struct Scene
  {
    std::vector<Material> materials;
    std::vector<Light> lights;
    std::vector<Camera> cameras;
    std::vector<Object> objects;
    Branch sceneGraph;
  };

  void buildScene(Scene & scene);
}

#endif
