#ifndef DMP_TEXTURE_HPP
#define DMP_TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include "../ext/stb_image.h"
#include "../util.hpp"

namespace dmp
{
  class Texture
  {
  public:
    Texture() = delete;
    Texture(const Texture &) = delete;
    Texture & operator=(const Texture &) = delete;
    Texture(Texture &&) = default;
    Texture & operator=(Texture &&) = default;

    Texture(std::string & path)
    {
      initTexture(path);
    }

    ~Texture() {}

    void freeTexture();

    operator GLuint() const
    {
      expect("texture valid prior to GLuint cast", mValid);
      return mTexId;
    }

  private:
    void initTexture(std::string &);
    GLuint mTexId = 0;
    bool mValid = false;
  };
}

#endif
