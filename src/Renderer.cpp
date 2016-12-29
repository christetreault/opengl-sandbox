#include "Renderer.hpp"

#include <set>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include "util.hpp"
#include "config.hpp"

void dmp::Renderer::initRenderer()
{
  expect("Init Glew",
         glewInit() == GLEW_OK);


}
