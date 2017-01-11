#include "Object.hpp"

#include <algorithm>

dmp::Object::Object(std::vector<ObjectVertex> verts,
                    GLenum format,
                    size_t matIdx,
                    size_t texIdx)
{
  mHasIndices = false;
  mPrimFormat = format;
  mMaterialIdx = matIdx;
  mTextureIdx = texIdx;

  initObject(&verts, nullptr);
}

dmp::Object::Object(std::vector<ObjectVertex> verts,
                    std::vector<GLuint> idxs,
                    GLenum format,
                    size_t matIdx,
                    size_t texIdx)
{
  mHasIndices = true;
  mPrimFormat = format;
  mMaterialIdx = matIdx;
  mTextureIdx = texIdx;

  initObject(&verts, &idxs);
}

void dmp::Object::initObject(std::vector<ObjectVertex> * verts,
                             std::vector<GLuint> * idxs)
{
  glGenVertexArrays(1,&mVAO);
  glGenBuffers(1, &mVBO);
  if (mHasIndices) glGenBuffers(1, &mEBO);

  expectNoErrors("Gen Buffers and Arrays");
  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER,
               verts->size() * sizeof(ObjectVertex),
               verts->data(),
               GL_STATIC_DRAW);

  drawCount = (GLsizei) verts->size();

  expectNoErrors("Upload Vertex data");

  if (mHasIndices)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   idxs->size() * sizeof(GLuint),
                   idxs->data(),
                   GL_STATIC_DRAW);

      drawCount = (GLsizei) idxs->size();

      expectNoErrors("Upload Index data");
    }

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,        // index
                        4,        // number of components
                        GL_FLOAT, // what is the type of this thing?
                        GL_FALSE, // normalize [intMin, intMax] to [-1,1]?
                        sizeof(ObjectVertex), // how much space between things?
                        (GLvoid *) 0);        // offset of this thing

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(ObjectVertex),
                        (GLvoid *) offsetof(ObjectVertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(ObjectVertex),
                        (GLvoid *) offsetof(ObjectVertex, texCoords));

  expectNoErrors("Set vertex attributes");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  expectNoErrors("Complete object init");
  mValid = true;
}

void dmp::sortByMaterial(std::vector<Object> & objs)
{
  std::sort(objs.begin(), objs.end(), [](const Object & lhs, const Object & rhs)
            {
              return lhs.mMaterialIdx < rhs.mMaterialIdx;
            });

  for (auto & curr : objs)
    {
      curr.mDirty = true;
    }
}
