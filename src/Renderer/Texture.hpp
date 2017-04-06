#ifndef DMP_TEXTURE_HPP
#define DMP_TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include "../util.hpp"
#include "../Image.hpp"

namespace dmp
{
  class Texture
  {
  public:
    Texture() = default;
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
    void initTexture(const std::string &);

    Image mImage;
    GLuint mTexId = 0;
    bool mValid = false;
    size_t mWidth;
    size_t mHeight;
    size_t mComponents;
  };

  inline GLuint texUnitAsInt(GLenum t) {return t - GL_TEXTURE0;}
}

#endif
