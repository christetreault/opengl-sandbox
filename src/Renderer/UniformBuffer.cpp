#include "UniformBuffer.hpp"

#include "../Renderer.hpp"

dmp::UniformBuffer::UniformBuffer(GLvoid * data,
                                  size_t elems,
                                  size_t elemSize,
                                  bool mut)
  : mMutable(mut), mElemSize((GLsizei) elemSize), mNumElems((GLsizei) elems)
{
  expect("if immutable, data must not be null!",
         mut || data);
  initUniformBuffer(data);
}

void dmp::UniformBuffer::initUniformBuffer(GLvoid * data)
{
  glGenBuffers(1, &mUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
  auto mutability = mMutable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
  glBufferData(GL_UNIFORM_BUFFER, mElemSize * mNumElems, data, mutability);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void dmp::UniformBuffer::update(size_t index, GLvoid * data)
{
  expect("This buffer is mutable", mMutable);
  expect("index in range", (GLsizei) index < mNumElems);
  glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
  glBufferSubData(GL_UNIFORM_BUFFER,
                  (GLsizei) index * mElemSize,
                  mElemSize,
                  data);
  glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
  expectNoErrors("Update buffer");
}

void dmp::UniformBuffer::bind(size_t blockIndex, size_t bufferIndex)
{
  expect("bufferIndex in range", bufferIndex < (size_t) mNumElems);

  auto alignment = uniformBufferOffsetAlignment();

  // TODO: blockIndex range check
  expect("alignmnent", (bufferIndex * mElemSize) % alignment == 0);
  glBindBufferRange(GL_UNIFORM_BUFFER,
                    (GLsizei) blockIndex,
                    mUBO,
                    (GLsizei) bufferIndex * mElemSize, // TODO: index = 1 * size is invalid
                    mElemSize);
}

size_t dmp::std140PadStruct(size_t size)
{
  size_t alignment = (size_t) uniformBufferOffsetAlignment();
  auto padMask = ~(alignment - 1);
  return (size + alignment - 1) & padMask;
}
