#include "Scene.hpp"

#include "Scene/Object.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "config.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;
using namespace dmp;

static void processMesh(std::vector<ObjectVertex> & verts,
                        std::vector<GLuint> & idxs,
                        aiMesh * mesh,
                        const aiScene * scene)
{
  for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
      ObjectVertex v =
        {
          {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z,
            1.0f
          },
          {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z,
            1.0f
          }
        };

      verts.push_back(v);
    }

  for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
      auto face = mesh->mFaces[i];
      if (face.mNumIndices != 3) continue; // not willing to open this can of worms...

      for (size_t j = 0; j < face.mNumIndices; ++j)
        {
          idxs.push_back((GLuint) face.mIndices[j]);
        }
    }
}

static void processNode(std::vector<ObjectVertex> & verts,
                        std::vector<GLuint> & idxs,
                        aiNode * node,
                        const aiScene * scene)
{
  for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
      aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
      processMesh(verts, idxs, /*mats,*/ mesh, scene);
    }

  for (size_t i = 0; i < node->mNumChildren; ++i)
    {
      processNode(verts, idxs, /*mats,*/ node->mChildren[i], scene);
    }
}

static void loadTestModel(std::vector<ObjectVertex> & verts,
                          std::vector<GLuint> & idxs)
{
  Importer imp;
  imp.SetPropertyBool(AI_CONFIG_PP_PTV_NORMALIZE, true);

  std::cerr << "loading: " << testModel << "..." << std::endl;
  auto model = imp.ReadFile(testModel,
                            aiProcess_Triangulate
                            | aiProcess_GenNormals
                            /*aiProcess_JoinIdenticalVertices
                            | aiProcess_Triangulate
                            | aiProcess_GenSmoothNormals
                            | aiProcess_PreTransformVertices
                            | aiProcess_ValidateDataStructure
                            | aiProcess_ImproveCacheLocality
                            | aiProcess_RemoveRedundantMaterials
                            | aiProcess_FindInvalidData
                            | aiProcess_GenUVCoords
                            | aiProcess_TransformUVCoords
                            | aiProcess_FindInstances
                            | aiProcess_OptimizeMeshes
                            | aiProcess_OptimizeGraph
                            | aiProcess_Debone*/);

  std::cerr << "done!" << std::endl;

  expect("Read model file",
         !(!model || model->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !model->mRootNode));

  processNode(verts, idxs, model->mRootNode, model);
}

void dmp::buildScene(dmp::Scene & scene)
{
  scene.graph = std::make_unique<Branch>();

  std::vector<ObjectVertex> triVerts(0);
  triVerts.push_back({glm::vec4(-1000.0f, 1000.0f, -100.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)});
  triVerts.push_back({glm::vec4(1000.0f, 1000.0f, -100.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)});
  triVerts.push_back({glm::vec4(0.0f, -1000.0f, -100.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)});
  std::vector<GLuint> triIdxs = {0,2,1};


  std::vector<ObjectVertex> verts(0);
  std::vector<GLuint> idxs(0);

  loadTestModel(verts, idxs);

  scene.objects.emplace_back(verts, GL_TRIANGLES, 0);
  scene.objects.emplace_back(triVerts, triIdxs, GL_TRIANGLES, 1);

  sortByMaterial(scene.objects);

  scene.materials.push_back(
    {
      {
        0.25f, 0.20725f, 0.20725f, 1.0f
      },
      {
        1.0f, 0.829f, 0.829f, 1.0f
      },
      {
        0.296648, 0.296648, 0.296648, 1.0f
      },
      0.088f
    });
  scene.materials.push_back(
    {
      {
        0.1745f, 0.01175f, 0.01175f, 1.0f
      },
      {
        0.61424f, 0.04136f, 0.04136f, 1.0f
      },
      {
        0.727811f, 0.626959f, 0.626959f, 1.0f
      },
      0.6f
    });

  scene.materialConstants =
    std::make_unique<UniformBuffer>(scene.materials.size(),
                                    Material::std140Size());

  for (size_t i = 0; i < scene.materials.size(); ++i)
    {
      scene.materialConstants->update(i,
                                      scene.materials[i]);
    }

  scene.lights.push_back({
      {1.0f, 1.0f, 1.0f, 1.0f},
      {0.0f, -0.1f, 1.0f, 0.0f},
      glm::mat4()
    });

  auto rotateY = scene.graph->transform([](glm::mat4 & M, float deltaT)
    {
      M = glm::rotate(M,
                      deltaT / 2.0f,
                      glm::vec3(0.0f, 1.0f, 0.0f));

      return true;
    });

  auto antiY = scene.graph->transform([](glm::mat4 & M, float deltaT)
    {
      M = glm::rotate(M,
                      -(deltaT / 2.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));

      return true;
    });

  rotateY->insert(scene.objects[0]);
  antiY->insert(scene.lights[0]);
  scene.graph->insert(scene.objects[1]);

  Camera cam;
  cam.pos = {0.0f, 0.0f, 600.0f, 1.0f};
  cam.up = {0.0f, 1.0f, 0.0f, 0.0f};
  cam.focus = {0.0f, 0.0f, 0.0f, 1.0f};

  scene.cameras.push_back(cam);
  scene.graph->insert(scene.cameras[0]);

  scene.objectConstants
    = std::make_unique<UniformBuffer>(scene.objects.size(),
                                      ObjectConstants::std140Size());

  scene.graph->update(0.0f, glm::mat4(), true);
}

void dmp::updateScene(dmp::Scene & scene,
                      float deltaT)
{
  expect("Object constant buffer not null",
         scene.objectConstants);

  scene.graph->update(deltaT);

  for (size_t i = 0; i < scene.objects.size(); ++i)
    {
      if (scene.objects[i].isDirty())
        {
          scene.objectConstants->update(i,
                                        scene.objects[i].getObjectConstants());
          scene.objects[i].setClean();
        }
    }
}

void dmp::freeScene(dmp::Scene & scene)
{
  for (auto & curr : scene.objects)
    {
      curr.freeObject();
    }
}
