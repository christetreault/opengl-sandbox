#include "UniformBuffer.hpp"

#include "../Renderer.hpp"

dmp::UniformBuffer::UniformBuffer(size_t elems,
                                  size_t elemSize)
  : mElemSize((GLsizei) elemSize), mNumElems((GLsizei) elems)
{
  initUniformBuffer();
}

void dmp::UniformBuffer::initUniformBuffer()
{
  glGenBuffers(1, &mUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
  glBufferData(GL_UNIFORM_BUFFER, mElemSize * mNumElems, nullptr,
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void dmp::UniformBuffer::update(size_t index, GLvoid * data)
{
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
                    (GLsizei) bufferIndex * mElemSize,
                    mElemSize);
}

size_t dmp::std140PadStruct(size_t size)
{
  size_t alignment = (size_t) uniformBufferOffsetAlignment();
  auto padMask = ~(alignment - 1);
  return (size + alignment - 1) & padMask;
}
