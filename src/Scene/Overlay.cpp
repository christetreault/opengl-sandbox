#include "Overlay.hpp"

void dmp::Overlay::initOverlay(float x,
                               float y,
                               float width,
                               float height,
                               Texture & tex)
{

  std::vector<OverlayVertex> verts;
  verts.push_back({{x, y},                  {0.0f, 0.0f}});
  verts.push_back({{x, y + height},         {0.0f, 1.0f}});
  verts.push_back({{x + width, y + height}, {1.0f, 1.0f}});

  verts.push_back({{x + width, y + height}, {1.0f, 1.0f}});
  verts.push_back({{x + width, y},          {1.0f, 0.0f}});
  verts.push_back({{x, y},                  {0.0f, 0.0f}});


}
