#ifndef DMP_UNIFORMBUFFER_HPP
#define DMP_UNIFORMBUFFER_HPP

#include <glm/glm.hpp>
#include <iostream>
#include "../util.hpp"

namespace dmp
{
  class UniformBuffer
  {
  public:
    UniformBuffer() = delete;
    UniformBuffer(const UniformBuffer &) = delete;
    UniformBuffer & operator=(const UniformBuffer &) = delete;
    UniformBuffer(UniformBuffer &&) = default;
    UniformBuffer & operator=(UniformBuffer &&) = default;

    UniformBuffer(size_t elems, size_t elemSize);
    void update(size_t index, GLvoid * data);
    void bind(size_t blockIndex, size_t bufferIndex);
    // TODO: void initializeData(std::vector<foo> data);
  private:
    void initUniformBuffer();
    GLuint mUBO = 0;
    GLsizei mElemSize = 0;
    GLsizei mNumElems = 0;
  };

  template <typename T>
  size_t std140ScalarSize()
  {
    return sizeof(T);
  }

  template <typename T, size_t len>
  size_t std140ScalarArraySize()
  {
    auto count = (len + 4) & 0xFFFFFFFC; // pad to a multiple of 4
    return count * std140ScalarSize<T>();
  }

  template <typename T, size_t len>
  size_t std140VecSize()
  {
    if (len == 2) return 2 * std140ScalarSize<T>();
    else if (len == 3 || len == 4) return 4 * std140ScalarSize<T>();
    else impossible("std140VecSize len not 2, 3, or 4!");
  }

  template <typename T, size_t dims, size_t len>
  size_t std140VecArraySize()
  {
    auto count = (len + 4) & 0xFFFFFFFC; // pad to a multiple of 4
    return count * std140VecSize<T, dims>();
  }

  template <typename T, size_t rows, size_t cols>
  size_t std140MatSize()
  {
    if (cols == 2) return 2 * std140VecSize<T, 2>();
    else if (cols == 3 || cols == 4) return 4 * std140VecSize<T, 4>();
    else impossible("std140MatSize cols not 2, 3, or 4!");
  }

  template <typename T, size_t rows, size_t cols, size_t len>
  size_t std140MatArraySize()
  {
    auto count = (len + 4) & 0xFFFFFFFC; // pad to a multiple of 4
    return count * std140MatSize<T, rows, cols>();
  }

  size_t std140PadStruct(size_t size);
}

#endif
