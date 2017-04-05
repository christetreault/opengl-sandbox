#ifndef DMP_SCENE_HPP
#define DMP_SCENE_HPP

#include <memory>

#include "Scene/Types.hpp"
#include "Scene/Object.hpp"
#include "Scene/Graph.hpp"
#include "Scene/Camera.hpp"
#include "Scene/Skybox.hpp"
#include "Renderer/UniformBuffer.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Overlay.hpp"

namespace dmp
{
  struct Scene
  {
    std::vector<Material> materials;
    std::unique_ptr<UniformBuffer> materialConstants;
    std::vector<Texture> textures;
    std::vector<Light> lights;
    std::vector<Camera> cameras;
    std::vector<Object *> objects;
    std::unique_ptr<UniformBuffer> objectConstants;
    std::unique_ptr<Branch> graph;
    std::unique_ptr<Skybox> skybox;
    std::vector<Overlay> overlays;
    std::unique_ptr<UniformBuffer> overlayConstants;

    void update(float deltaT);
    void free();
  };
}

#endif
