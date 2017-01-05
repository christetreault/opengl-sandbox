#ifndef DMP_PROGRAM_HPP
#define DMP_PROGRAM_HPP

#include "Window.hpp"
#include "Renderer.hpp"
#include "util.hpp"
#include "Timer.hpp"
#include "Scene.hpp"

namespace dmp
{
  class Program
  {
  public:
    Program() = delete;
    ~Program() = default;
    Program(const Program &) = delete;
    Program & operator=(const Program &) = delete;
    Program(Program &&) = default;
    Program & operator=(Program &&) = default;

    Program(int width, int height, const char * title);
    int run();
  private:
    Window mWindow;
    Renderer mRenderer;
    Timer mTimer;
    Scene mScene;
  };
}
#endif
