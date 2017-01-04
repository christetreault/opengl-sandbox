#ifndef DMP_SCENE_GRAPH_HPP
#define DMP_SCENE_GRAPH_HPP

#include <boost/variant.hpp>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "Object.hpp"

namespace dmp
{
  class Node
  {
  public:
    virtual ~Node() {}
    void update(float deltaT,
                glm::mat4 M = glm::mat4(),
                bool dirty = false) {updateImpl(deltaT, M, dirty);}
  private:
    virtual void updateImpl(float deltaT, glm::mat4 M, bool dirty) = 0;
  };

  class Branch : public Node
  {
  public:
    std::vector<std::unique_ptr<Node>> mChildren;
  private:
    void updateImpl(float deltaT, glm::mat4 M, bool dirty) override
    {
      for (auto & curr : mChildren)
        {
          curr->update(deltaT, M, dirty);
        }
    }
  };

  static auto identityTransFn = [] (glm::mat4 &, float) {return false;};

  class Transform : public Node
  {
  public:
    std::function<bool(glm::mat4 &, float)> mUpdateFn = identityTransFn;
    glm::mat4 mTransform;
    std::unique_ptr<Node> mChild = nullptr;
  private:
    void updateImpl(float deltaT, glm::mat4 M, bool) override
    {
      // If outDirty == false, then it must be the case that mTransform is not
      // changed
      bool outDirty = mUpdateFn(mTransform, deltaT);
      glm::mat4 outM = M * mTransform;
      if (mChild) mChild->update(deltaT, outM, outDirty);
    }
  };

  class Container : public Node
  {
  public:
    Container() = delete;
    Container(Object & obj)
      : mValue(obj)
    {}

  private:
    void updateImpl(float, glm::mat4 M, bool dirty) override
    {
      if (dirty)
        {
          boost::apply_visitor([&](auto & v)
                               {
                                 v.setDirty();
                                 v.setM(M);
                               }, mValue);
        }
    }

    boost::variant<Object &> mValue;
  };


}

#endif
