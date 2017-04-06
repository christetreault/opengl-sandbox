#include "Image.hpp"
#include "util.hpp"
#include "ext/stb_image.h"
#include "ext/stb_image_write.h"
#include <iostream>
#include <GL/glew.h>

dmp::Image::Image(std::string path)
{
  initImage(path.c_str());
}

dmp::Image::Image(const char * path)
{
  initImage(path);
}

dmp::Image::Image()
{
  width = 1;
  height = 1;
  channels = REQUESTED_CHANNELS;

  data =
    {
      (unsigned char) DEFAULT_COLOR,
      (unsigned char) DEFAULT_COLOR,
      (unsigned char) DEFAULT_COLOR,
      (unsigned char) DEFAULT_COLOR
    };
}

void dmp::Image::initImage(const char * path)
{
  expect("Path not null", path);

  int width, height, channels;
  unsigned char * pixels = stbi_load(path,
                                     &width,
                                     &height,
                                     &channels,
                                     REQUESTED_CHANNELS);

  ifDebug(if (pixels == nullptr)
            {
              std::cerr << "Failed to load image: "
                        << stbi_failure_reason()
                        << std::endl;
            });
  expect("Load image file", pixels);

  data.resize(width * height * REQUESTED_CHANNELS);

  for (size_t i = 0; i < data.size(); ++i)
    {
      data[i] = pixels[i];
    }
  stbi_image_free(pixels);

  this->width = (size_t) width;
  this->height = (size_t) height;
  this->channels = (size_t) channels;
}

dmp::Image::Image(int x, int y, int w, int h)
{
  width = (size_t) w;
  height = (size_t) h;
  channels = REQUESTED_CHANNELS;
  data.resize(width * height * channels);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadnPixels(x, viewport[3] - y,
                (GLsizei) width,
                (GLsizei) height,
                GL_RGBA, // TODO: might not always be this
                GL_UNSIGNED_BYTE,
                (GLsizei) data.size(),
                data.data());

  expectNoErrors("Read from framebuffer");
}

void dmp::Image::writeBMPToFile(std::string path) const
{
  writeBMPToFile(path.c_str());
}

void dmp::Image::writeBMPToFile(const char * path) const
{
  expect("Path not null", path);

  auto res = stbi_write_bmp(path,
                            (int) width,
                            (int) height,
                            (int) channels,
                            data.data());

  expect("Successfully wrote file", res != 0);
}
