#ifndef DMP_SCENE_OBJECT_HPP
#define DMP_SCENE_OBJECT_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Types.hpp"
#include "../util.hpp"
#include "../Renderer/UniformBuffer.hpp"

#include <iostream>

namespace dmp
{
  struct ObjectVertex
  {
    glm::vec4 position;
    glm::vec4 normal;
  };

  struct ObjectConstants
  {
    glm::mat4 M;
    glm::mat4 normalM;
    static size_t std140Size()
    {
      return dmp::std140PadStruct(std140MatSize<float, 4, 4>() * 2);
    }

    operator GLvoid *() {return (GLvoid *) this;}
  };

  class Object
  {
  public:
    Object() = delete;
    Object(const Object &) = delete;
    Object & operator=(const Object &) = delete;
    Object(Object &&) = default;
    Object & operator=(Object &&) = default;

    ~Object() {}

    void freeObject()
    {
      if (!mValid) return;

      glDeleteVertexArrays(1, &mVAO);
      glDeleteBuffers(1, &mVBO);
      if (mHasIndices) glDeleteBuffers(1, &mEBO);
    }

    Object(std::vector<ObjectVertex> verts,
           GLenum format,
           size_t matIdx);
    Object(std::vector<ObjectVertex> verts,
           std::vector<GLuint> idxs,
           GLenum format,
           size_t matIdx);

    void setDirty() {mDirty = true;}
    bool isDirty() const {return mDirty;}
    void setClean() {mDirty = false;}
    void setM(glm::mat4 M) {mM = M;}

    void bind() const
    {
      expect("Object valid", mValid);
      expectNoErrors("Pre-bind object");
      glBindVertexArray(mVAO);
      expectNoErrors("Bind object");
    }

    void draw() const
    {
      expect("Object valid", mValid);
      if (mHasIndices)
        {
          glDrawElements(mPrimFormat,
                         drawCount,
                         GL_UNSIGNED_INT,
                         0); // TODO: whats up with this parameter? (its a pointer)
        }
      else
        {
          glDrawArrays(mPrimFormat,
                       0,
                       drawCount);
        }
      expectNoErrors("Draw object");
    }

    ObjectConstants getObjectConstants() const
    {
      ObjectConstants retVal =
        {
          mM,
          glm::mat4(glm::transpose(glm::inverse(glm::mat3(mM))))
        };

      return retVal;
    }

    glm::mat4 getM() const {return mM;}

    size_t materialIndex() const {return mMaterialIdx;}

  private:
    void initObject(std::vector<ObjectVertex> * verts,
                    std::vector<GLuint> * idxs);

    GLuint mVAO = 0;
    GLuint mVBO = 0;
    GLuint mEBO = 0;

    bool mDirty = true;
    glm::mat4 mM;
    bool mHasIndices;
    GLenum mPrimFormat;
    bool mValid = false;
    size_t mMaterialIdx;

    GLsizei drawCount;

    friend void sortByMaterial(std::vector<Object> & objs);
  };

  void sortByMaterial(std::vector<Object> & objs);
}

#endif
