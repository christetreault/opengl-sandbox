#include "Graph.hpp"

using namespace dmp;

// -----------------------------------------------------------------------------
// ContainerVisitor
// -----------------------------------------------------------------------------

void ContainerVisitor::operator()(Object & obj) const
{
  obj.setDirty();
  obj.setM(mM);
}

void ContainerVisitor::operator()(Camera & cam) const
{
  cam.pos = mM * cam.pos;
  cam.focus = mM * cam.focus;
  cam.V = glm::lookAt(glm::vec3(cam.pos),
                      glm::vec3(cam.focus),
                      glm::vec3(cam.up));
}

void ContainerVisitor::operator()(Light & lit) const
{
  lit.M = mM;
}

// -----------------------------------------------------------------------------
// Container
// -----------------------------------------------------------------------------

void Container::updateImpl(float deltaT, glm::mat4 M, bool dirty)
{
  if (dirty)
    {
      boost::apply_visitor(ContainerVisitor(deltaT, M, dirty), mValue);
    }
}

// -----------------------------------------------------------------------------
// Transform
// -----------------------------------------------------------------------------

Container * Transform::insert(Object & o)
{
  mChild = std::make_unique<Container>(o);
  return (Container *) mChild.get();
}

Container * Transform::insert(Light & l)
{
  mChild = std::make_unique<Container>(l);
  return (Container *) mChild.get();
}

Container * Transform::insert(Camera & c)
{
  mChild = std::make_unique<Container>(c);
  return (Container *) mChild.get();
}

Node * Transform::insert(std::unique_ptr<Node> & n)
{
  mChild = std::move(n);
  return mChild.get();
}

Transform * Transform::insert(std::unique_ptr<Transform> & t)
{
  mChild = std::move(t);
  return (Transform *) mChild.get();
}

Transform * Transform::transform()
{
  auto p = std::make_unique<Transform>();
  return insert(p);
}

Transform * Transform::transform(glm::mat4 & t)
{
  auto p = std::make_unique<Transform>();
  p->mTransform = t;
  return insert(p);
}

Transform * Transform::transform(std::function<bool(glm::mat4 &, float)> f)
{
  auto p = std::make_unique<Transform>();
  p->mUpdateFn = f;
  return insert(p);
}

Transform * Transform::transform(glm::mat4 & t,
                                 std::function<bool(glm::mat4 &, float)> f)
{
  auto p = std::make_unique<Transform>();
  p->mTransform = t;
  p->mUpdateFn = f;
  return insert(p);
}

Branch * Transform::insert(std::unique_ptr<Branch> & b)
{
  mChild = std::move(b);
  return (Branch *) mChild.get();
}

Branch * Transform::branch()
{
  auto p = std::make_unique<Branch>();
  return insert(p);
}

Container * Transform::insert(std::unique_ptr<Container> & c)
{
  mChild = std::move(c);
  return (Container *) mChild.get();
}

// -----------------------------------------------------------------------------
// Branch
// -----------------------------------------------------------------------------

Node * Branch::insert(std::unique_ptr<Node> & n)
{
  mChildren.push_back(std::move(n));
  return mChildren.back().get();
}

Transform * Branch::insert(std::unique_ptr<Transform> & t)
{
  mChildren.push_back(std::move(t));
  return (Transform *) mChildren.back().get();
}

Transform * Branch::transform()
{
  auto p = std::make_unique<Transform>();
  return insert(p);
}

Transform * Branch::transform(glm::mat4 & t)
{
  auto p = std::make_unique<Transform>();
  p->mTransform = t;
  return insert(p);
}

Transform * Branch::transform(std::function<bool(glm::mat4 &, float)> f)
{
  auto p = std::make_unique<Transform>();
  p->mUpdateFn = f;
  return insert(p);
}

Transform * Branch::transform(glm::mat4 & t,
                              std::function<bool(glm::mat4 &, float)> f)
{
  auto p = std::make_unique<Transform>();
  p->mTransform = t;
  p->mUpdateFn = f;
  return insert(p);
}

Branch * Branch::insert(std::unique_ptr<Branch> & b)
{
  mChildren.push_back(std::move(b));
  return (Branch *) mChildren.back().get();
}

Container * Branch::insert(std::unique_ptr<Container> & c)
{
  mChildren.push_back(std::move(c));
  return (Container *) mChildren.back().get();
}

Container * Branch::insert(Object & o)
{
  mChildren.push_back(std::make_unique<Container>(o));
  return (Container *) mChildren.back().get();
}

Container * Branch::insert(Light & l)
{
  mChildren.push_back(std::make_unique<Container>(l));
  return (Container *) mChildren.back().get();
}

Container * Branch::insert(Camera & c)
{
  mChildren.push_back(std::make_unique<Container>(c));
  return (Container *) mChildren.back().get();
}
