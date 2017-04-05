#include "Overlay.hpp"

dmp::Overlay::Overlay(float x,
                      float y,
                      float width,
                      float height,
                      int id,
                      Texture & tex)
{
  initOverlay(x, y, width, height, static_cast<int32_t>(id), tex);
}

dmp::Overlay::Overlay(float x,
                      float y,
                      float width,
                      float height,
                      Texture & tex)
{
  initOverlay(x, y, width, height, 255, tex);
}

void dmp::Overlay::initOverlay(float x,
                               float y,
                               float width,
                               float height,
                               int id,
                               Texture & tex)
{
  // enforce invariants
  expect("X within [-1.0, 1.0]",
         x <= 1.0f  && x >= -1.0f);
  expect("Y within [-1.0, 1.0]",
         y <= 1.0f  && y >= -1.0f);
  expect("Width within [0, 2.0]",
         width <= 2.0f && width >= 0.0f);
  expect("Height within [0, 2.0]",
         height <= 2.0f && height >= 0.0f);
  expect("ID within [0, 255]",
         id <= 255 && id >= 0);

  // init the overlay
  std::vector<OverlayVertex> verts;
  verts.push_back({{x, y},                  {0.0f, 0.0f}});
  verts.push_back({{x, y - height},         {0.0f, 1.0f}});
  verts.push_back({{x + width, y - height}, {1.0f, 1.0f}});

  verts.push_back({{x + width, y - height}, {1.0f, 1.0f}});
  verts.push_back({{x + width, y},          {1.0f, 0.0f}});
  verts.push_back({{x, y},                  {0.0f, 0.0f}});

  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);

  expectNoErrors("Gen Overlay buffers and Arrays");
  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER,
               verts.size() * sizeof(OverlayVertex),
               verts.data(),
               GL_STATIC_DRAW);

  expectNoErrors("Upload Overlay vertex data");

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(OverlayVertex),
                        (GLvoid *) 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(OverlayVertex),
                        (GLvoid *) offsetof(OverlayVertex, texCoords));

  expectNoErrors("Set Overlay vertex attributes");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  mTexture = std::move(tex);
  mID = id;

  expectNoErrors("Complete Overlay init");
  mValid = true;
}

dmp::OverlayConstants dmp::Overlay::getOverlayConstants() const
{
  OverlayConstants oc;
  oc.overlayID = mID;
  return oc;
}

void dmp::Overlay::draw() const
{
  expect("Overlay valid", mValid);
  if (!mVisible) return;
  expectNoErrors("Pre-bind overlay");
  glBindVertexArray(mVAO);
  expectNoErrors("Bind overlay");
  glDrawArrays(GL_TRIANGLES,
               0, Overlay::drawCount);
}

void dmp::Overlay::freeOverlay()
{
  if (!mValid) return;

  glDeleteVertexArrays(1, &mVAO);
  glDeleteBuffers(1, &mVBO);

  mValid = false;
}
