#ifndef DMP_SCENE_GRAPH_HPP
#define DMP_SCENE_GRAPH_HPP

#include <boost/variant.hpp>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include "Object.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace dmp
{
  class Node
  {
  public:
    virtual ~Node() {}
    void update(float deltaT = 0.0f,
                glm::mat4 M = glm::mat4(),
                bool dirty = false) {updateImpl(deltaT, M, dirty);}
  private:
    virtual void updateImpl(float deltaT, glm::mat4 M, bool dirty) = 0;
  };

  class Branch;
  class Container;
  class Transform;

  class ContainerVisitor : public boost::static_visitor<>
  {
  public:
    ContainerVisitor(float deltaT, glm::mat4 M, bool dirty)
      : mDeltaT(deltaT), mM(M), mDirty(dirty) {}

    void operator()(Object & obj) const
    {
      obj.setDirty();
      obj.setM(mM);
    }
    void operator()(Camera & cam) const
    {
      cam.pos = mM * cam.pos;
      cam.focus = mM * cam.focus;
      cam.V = glm::lookAt(glm::vec3(cam.pos),
                          glm::vec3(cam.focus),
                          glm::vec3(cam.up));
    }

    void operator()(Light & lit) const
    {
    }

    float mDeltaT;
    glm::mat4 mM;
    bool mDirty;
  };

  class Container : public Node
  {
  public:
    Container() = delete;
    Container(Object & obj)
      : mValue(obj)
    {}
    Container(Camera & cam)
      : mValue(cam)
    {}
    Container(Light & lit)
      : mValue(lit)
    {}

  private:
    void updateImpl(float deltaT, glm::mat4 M, bool dirty) override
    {
      if (dirty)
        {
          boost::apply_visitor(ContainerVisitor(deltaT, M, dirty), mValue);
        }
    }

    boost::variant<Object &, Camera &, Light &> mValue;
  };

  static auto identityTransFn = [] (glm::mat4 &, float) {return false;};

  class Transform : public Node
  {
  public:
    std::function<bool(glm::mat4 &, float)> mUpdateFn = identityTransFn;
    glm::mat4 mTransform;
    std::unique_ptr<Node> mChild = nullptr;
    void insert(Object & o) {mChild = std::make_unique<Container>(o);}
    void insert(Light & l) {mChild = std::make_unique<Container>(l);}
    void insert(Camera & c) {mChild = std::make_unique<Container>(c);}
    void insert(std::unique_ptr<Node> & n) {mChild = std::move(n);}
    void insert(std::unique_ptr<Transform> & t) {mChild = std::move(t);}
    void insert(std::unique_ptr<Branch> & b);
    void insert(std::unique_ptr<Container> & c) {mChild = std::move(c);}
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

  class Branch : public Node
  {
  public:
    void insert(std::unique_ptr<Node> & n) {mChildren.push_back(std::move(n));}
    void insert(std::unique_ptr<Transform> & t) {mChildren.push_back(std::move(t));}
    void insert(std::unique_ptr<Branch> & b) {mChildren.push_back(std::move(b));}
    void insert(std::unique_ptr<Container> & c) {mChildren.push_back(std::move(c));}
    void insert(Object & o) {mChildren.push_back(std::make_unique<Container>(o));}
    void insert(Light & l) {mChildren.push_back(std::make_unique<Container>(l));}
    void insert(Camera & c) {mChildren.push_back(std::make_unique<Container>(c));}
  private:
    std::vector<std::unique_ptr<Node>> mChildren;
    void updateImpl(float deltaT, glm::mat4 M, bool dirty) override
    {
      for (auto & curr : mChildren)
        {
          curr->update(deltaT, M, dirty);
        }
    }
  };

  // this is super gross. I just need to stop being lazy and make a .cpp file...
  inline void Transform::insert(std::unique_ptr<Branch> & b)
  {
    mChild = std::move(b);
  }
}

#endif
