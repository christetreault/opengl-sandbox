#ifndef DMP_SCENE_GRAPH_HPP
#define DMP_SCENE_GRAPH_HPP

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include "Object.hpp"
#include "Camera.hpp"
#include <glm/gtc/quaternion.hpp>


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

    void operator()(Object & obj) const;
    void operator()(CameraPos & cam) const;
    void operator()(CameraFocus & cam) const;
    void operator()(Light & lit) const;

    float mDeltaT;
    glm::mat4 mM;
    bool mDirty;
  };

  class Container : public Node
  {
  public:
    Container() = delete;
    Container(Object obj) : mValue(obj) {}
    Container(CameraPos & cam) : mValue(cam) {}
    Container(CameraFocus & cam) : mValue(cam) {}
    Container(Light & lit) : mValue(lit) {}
    boost::variant<Object, CameraPos &, CameraFocus &, Light &> mValue;
  private:
    void updateImpl(float deltaT, glm::mat4 M, bool dirty) override;

  };

  static auto noTransform = [] (glm::mat4 &, glm::quat &, float)
  {
    boost::optional<glm::mat4> res;
    return res;
  };

  typedef std::function<boost::optional<glm::mat4>(glm::mat4 &,
                                                   glm::quat &,
                                                   float)> TransformFn;

  class Transform : public Node
  {
  public:
    TransformFn mUpdateFn = noTransform;
    glm::mat4 mTransformResult;
    glm::mat4 mMatrixTransformState;
    glm::quat mQuatRotationState;
    std::unique_ptr<Node> mChild = nullptr;

    Object * insert(Object o);
    Light * insert(Light & l);
    CameraPos * insert(CameraPos & c);
    CameraFocus * insert(CameraFocus & c);
    Node * insert(std::unique_ptr<Node> & n);
    Transform * insert(std::unique_ptr<Transform> & t);
    Branch * insert(std::unique_ptr<Branch> & b);
    Container * insert(std::unique_ptr<Container> & c);

    Transform * transform();
    Transform * transform(glm::mat4);
    Transform * transform(TransformFn);
    Transform * transform(glm::quat, TransformFn);
    Transform * transform(glm::mat4, TransformFn);
    Transform * transform(glm::quat, glm::mat4, TransformFn);
    Branch * branch();
  private:
    void updateImpl(float deltaT, glm::mat4 M, bool inDirty) override;
  };

  class Branch : public Node
  {
  public:
    std::vector<std::unique_ptr<Node>> mChildren;

    Node * insert(std::unique_ptr<Node> & n);
    Transform * insert(std::unique_ptr<Transform> & t);
    Branch * insert(std::unique_ptr<Branch> & b);
    Container * insert(std::unique_ptr<Container> & c);
    Object * insert(Object o);
    Light * insert(Light & l);
    CameraPos * insert(CameraPos & c);
    CameraFocus * insert(CameraFocus & c);

    Transform * transform();
    Transform * transform(glm::mat4);
    Transform * transform(TransformFn);
    Transform * transform(glm::quat, TransformFn);
    Transform * transform(glm::mat4, TransformFn);
    Transform * transform(glm::quat, glm::mat4, TransformFn);
  private:
    void updateImpl(float deltaT, glm::mat4 M, bool dirty) override
    {
      for (auto & curr : mChildren)
        {
          expect("branch not null", curr);
          if (curr) curr->update(deltaT, M, dirty);
        }
    }
  };
}

#endif
