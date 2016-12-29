#ifndef DMP_RINGBUFFER_HPP
#define DMP_RINGBUFFER_HPP

#include <array>

namespace dmp
{
  template <typename T, unsigned int Size> class RingBuffer
  {
  public:
    RingBuffer() {};

    T * operator&() {return &mData[mCurr];}
    operator T&() { return mData[mCurr];}
    T & operator[](size_t i) {return mData[i];}

    size_t size() {return (size_t) Size;}

    void next() {mCurr = (mCurr + 1) % Size;}

  private:
    std::array<T, Size> mData;
    size_t mCurr = 0;
  };
}

#endif
