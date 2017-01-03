#include "Program.hpp"

#include <iostream>

static void errorCb(int error, const char * description)
{
  std::cerr << "GLFW Error "
            << error
            << ": "
            << description
            << std::endl;
}

dmp::Program::Program(int width, int height, const char * title)
  : mWindow(width, height, title),
    mRenderer((GLsizei) width, (GLsizei) height),
    mTimer()
{
  mWindow.keyFn = [](GLFWwindow * w,
                     int key,
                     int scancode,
                     int action,
                     int mods)
    {
      if (action == GLFW_PRESS)
        {
          switch (key)
            {
            case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(w, true);
            break;
            }
        }
    };

  glfwSetErrorCallback(errorCb);
}

int dmp::Program::run()
{
  while (!mWindow.shouldClose())
    {
      glfwPollEvents();
    }
  return EXIT_SUCCESS;
}
