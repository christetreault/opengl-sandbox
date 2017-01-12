#include "Texture.hpp"

#include "../util.hpp"
#include "../ext/stb_image.h"
#include <iostream>

void dmp::Texture::initTexture(std::string & path)
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

  int width, height, channels;
  unsigned char noTex[4] =
    {
      (unsigned char) 0xFF,
      (unsigned char) 0xFF,
      (unsigned char) 0xFF,
      (unsigned char) 0xFF
    };

  unsigned char * data;
  if (path == "")
    {
      data = noTex;
      width = 1;
      height = 1;
      channels = 4;
    }
  else
    {
      data = stbi_load(path.c_str(),
                       &width,
                       &height,
                       &channels,
                       4); // TODO: revisit this
    }

  ifDebug(if (data == nullptr)
            {
              std::cerr << "Failed to load image: "
                        << stbi_failure_reason()
                        << std::endl;
            });

  expect("Load image file", data);

  glTexImage2D(GL_TEXTURE_2D,
               0, // generating mipmaps
               GL_RGBA, // forced an alpha channel
               width,
               height,
               0, // should always be zero because "legacy"
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               data);
  expectNoErrors("generate texture");

  glGenerateMipmap(GL_TEXTURE_2D);
  expectNoErrors("generate mipmaps");

  if (path != "") stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  expectNoErrors("initialize Texture");
  mValid = true;
}

void dmp::Texture::freeTexture()
{
  expect("texture valid prior to delete", mValid);
  glDeleteTextures(1, &mTexId);
}
