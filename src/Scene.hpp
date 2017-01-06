#ifndef DMP_SCENE_HPP
#define DMP_SCENE_HPP

#include <memory>

#include "Scene/Types.hpp"
#include "Scene/Object.hpp"
#include "Scene/Graph.hpp"
#include "Renderer/UniformBuffer.hpp"

namespace dmp
{
  struct Scene
  {
    std::vector<Material> materials;
    std::vector<Light> lights;
    std::vector<Camera> cameras;
    std::vector<Object> objects;
    std::unique_ptr<UniformBuffer> objectConstants;
    Branch sceneGraph;
  };

  void buildScene(Scene & scene);
  void updateScene(Scene & scene,
                   float deltaT);
  void freeScene(Scene & scene);
}

#endif
