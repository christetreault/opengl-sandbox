#include "Window.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include "util.hpp"

void dmp::Window::initCallbacks()
{
  glfwSetWindowUserPointer(mWindow, this);

  glfwSetMouseButtonCallback(mWindow, mouseButton);
  mouseButtonFn = [](GLFWwindow *, int, int, int){};
  glfwSetCursorPosCallback(mWindow, cursorPos);
  cursorPosFn = [](GLFWwindow *, double, double){};
  glfwSetCursorEnterCallback(mWindow, cursorEnter);
  cursorEnterFn = [](GLFWwindow *, int){};
  glfwSetScrollCallback(mWindow, scroll);
  scrollFn = [](GLFWwindow *, double, double){};
  glfwSetKeyCallback(mWindow, key);
  keyFn = [](GLFWwindow *, int, int, int, int){};

  glfwSetWindowPosCallback(mWindow, windowPos);
  windowPosFn = [](GLFWwindow *, int, int){};
  glfwSetWindowSizeCallback(mWindow, windowSize);
  windowSizeFn = [](GLFWwindow *, int, int){};
  glfwSetWindowCloseCallback(mWindow, windowClose);
  windowCloseFn = [](GLFWwindow *){};
  glfwSetWindowRefreshCallback(mWindow, windowRefresh);
  windowRefreshFn = [](GLFWwindow *){};
  glfwSetWindowFocusCallback(mWindow, windowFocus);
  windowFocusFn = [](GLFWwindow *, int){};
  glfwSetWindowIconifyCallback(mWindow, windowIconify);
  windowIconifyFn = [](GLFWwindow *, int){};
  glfwSetFramebufferSizeCallback(mWindow, windowFrameBufferResize);
  windowFrameBufferResizeFn = [](GLFWwindow *, int, int){};
}

dmp::Window::Window(int width,
                    int height,
                    const char * title)
{
  auto working = glfwCreateWindow(width, height, title, nullptr, nullptr);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  ifDebug(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE));
  ifRelease(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE));

  expect("Window creation failed!",
         working != nullptr);

  mWindow = working;
  initCallbacks();

  glfwMakeContextCurrent(mWindow);

  glfwSwapInterval(1); // vsync
}

dmp::Window::~Window()
{
  glfwDestroyWindow(mWindow);
}
