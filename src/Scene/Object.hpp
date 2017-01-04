#ifndef DMP_SCENE_OBJECT_HPP
#define DMP_SCENE_OBJECT_HPP

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../Types.hpp"
#include "../util.hpp"

namespace dmp
{
  struct ObjectVertex
  {
    glm::vec4 position;
    glm::vec4 normal;
  };

  class Object
  {
  public:
    Object() = delete;
    Object(const Object &) = delete;
    Object & operator=(const Object &) = delete;
    Object(Object &&) = default;
    Object & operator=(Object &&) = default;

    ~Object()
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
    void setM(glm::mat4 M) {mM = M;}

    void bind() const
    {
      expect("Object valid", mValid);
      glBindVertexArray(mVAO);
      expectNoErrors("Bind object");
    }

    void draw()
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
