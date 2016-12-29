#ifndef DMP_RENDERER_HPP
#define DMP_RENDERER_HPP

namespace dmp
{
  class Renderer
  {
  public:
    //Renderer() = delete;
    Renderer(const Renderer &) = delete;
    Renderer & operator=(const Renderer &) = delete;
    Renderer(Renderer && other) = default;
    Renderer & operator=(Renderer && other) = default;

    Renderer();
  private:
    void initRenderer();
  };
};

#endif
