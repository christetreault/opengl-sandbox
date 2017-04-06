#ifndef DMP_IMAGE_HPP
#define DMP_IMAGE_HPP

#include <vector>
#include <cstddef>
#include <string>

namespace dmp
{
  class Image
  {
  public:
    static const unsigned char DEFAULT_COLOR = 0xFF;
    static const int REQUESTED_CHANNELS = 4;

    Image(const Image &) = default;
    Image & operator=(const Image &) = default;

    Image();
    Image(std::string path);
    Image(const char * path);
    Image(int x, int y, int width, int height);

    void writeBMPToFile(const char * path) const;
    void writeBMPToFile(std::string path) const;

    // Fields

    size_t width;
    size_t height;
    size_t channels;
    std::vector<unsigned char> data;
  private:
    void initImage(const char * path);
  };
}

#endif
