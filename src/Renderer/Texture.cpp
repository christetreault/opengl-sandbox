#include "Texture.hpp"

#include "../util.hpp"

#include <iostream>

void dmp::Texture::initTexture(const std::string & path)
{
  glGenTextures(1, &mTexId);
  expectNoErrors("gen texture");

  glBindTexture(GL_TEXTURE_2D, mTexId);
  expectNoErrors("bind texture");

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  expectNoErrors("texture wrapping");

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  expectNoErrors("texture filtering");

  if (path != "")
    {
      mImage = Image(path);
    }

  // TODO: gracefully handle the possibility that Image might not be RGBA
  glTexImage2D(GL_TEXTURE_2D,
               0, // generating mipmaps
               GL_RGBA, // forced an alpha channel (TODO)
               (GLsizei) mImage.width,
               (GLsizei) mImage.height,
               0, // should always be zero because "legacy"
               GL_RGBA, // (TODO?)
               GL_UNSIGNED_BYTE,
               mImage.data.data());

  expectNoErrors("generate texture");

  glGenerateMipmap(GL_TEXTURE_2D);
  expectNoErrors("generate mipmaps");

  glBindTexture(GL_TEXTURE_2D, 0);

  expectNoErrors("initialize Texture");
  mValid = true;
}

void dmp::Texture::freeTexture()
{
  expect("texture valid prior to delete", mValid);
  glDeleteTextures(1, &mTexId);
}
