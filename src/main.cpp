#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.hpp"
#include "Renderer.hpp"
#include "util.hpp"


static void errorCb(int error, const char * description)
{
  std::cerr << "GLFW Error "
            << error
            << ": "
            << description
            << std::endl;
}

int main(int, char **)
{
  using namespace dmp;

  ifDebug(std::cerr << "Built in debug mode..." << std::endl);
  ifRelease(std::cerr << "Built in release mode..." << std::endl);

  try
    {
      expect("GLFW init failed!",
             glfwInit());

      int width = 512;
      int height = 512;
      Window win(width, height, "Petting a cat's tummy is dangerous,"
                 "but nothing ventured nothing gained");
      win.keyFn = [](GLFWwindow * w,
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

      while (!win.shouldClose())
        {
          glfwPollEvents();
        }
    }
  catch (dmp::InvariantViolation & e)
    {
      std::cerr << "Invariant Violation!"
                << std::endl
                << "--------------------------------------------------------------------------------"
                << std::endl
                << e.what()
                << std::endl
                << "--------------------------------------------------------------------------------"
                << std::endl;
    }
  glfwTerminate();
  return EXIT_SUCCESS;
}
