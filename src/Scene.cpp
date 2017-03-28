#include "Scene.hpp"

#include "Scene/Object.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "config.hpp"

void dmp::Scene::update(float deltaT)
{
  expect("Object constant buffer not null",
         objectConstants);

  graph->update(deltaT);

  for (size_t i = 0; i < objects.size(); ++i)
    {
      if (objects[i]->isDirty())
        {
          objectConstants->update(i, objects[i]->getObjectConstants());
          objects[i]->setClean();
        }
    }

  for (auto & curr : cameras)
    {
      curr.update();
    }
}

void dmp::Scene::free()
{
  for (auto & curr : objects)
    {
      curr->freeObject();
    }

  for (auto & curr : textures)
    {
      curr.freeTexture();
    }

  skybox->freeSkybox();
}
