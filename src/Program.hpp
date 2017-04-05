#ifndef DMP_PROGRAM_HPP
#define DMP_PROGRAM_HPP

#include <map>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Window.hpp"
#include "Renderer.hpp"
#include "util.hpp"
#include "Timer.hpp"
#include "Scene.hpp"

namespace dmp
{
  struct Keybind
  {
    int key = 0;
    int action = 0;
    int scancode = 0;
    int mods = 0;
    GLFWwindow * window = nullptr;
    std::function<void(Keybind &)> fn = [](Keybind &){}; // nop by default

    Keybind() = default;

    Keybind(GLFWwindow * w,
            std::function<void(Keybind &)> f,
            int k,
            int a = GLFW_PRESS,
            int m = 0,
            int s = 0)
    {
      key = k;
      action = a;
      mods = m;
      scancode = s;
      fn = f;
      window = w;
    }

    size_t hash() const
    {
      size_t h = 0;
      if (key != GLFW_KEY_UNKNOWN) boost::hash_combine(h, key);
      else boost::hash_combine(h, scancode);

      boost::hash_combine(h, action);
      boost::hash_combine(h, mods);
      boost::hash_combine(h, window);

      return h;
    }

    void print(std::string name = "") const
    {
      std::cerr << name << " = {key = "
                << key << ", action = "
                << action << ", mods = "
                << mods << ", scancode = "
                << scancode << ", hash = "
                << hash() << "}" << std::endl;
    }
  };

  inline bool operator==(Keybind lhs, Keybind rhs)
    {
      bool keyMatch;
      if (lhs.key != GLFW_KEY_UNKNOWN) keyMatch = lhs.key == rhs.key;
      else keyMatch = lhs.scancode == rhs.scancode;

      bool actionMatch = lhs.action == rhs.action;
      bool modsMatch  = lhs.mods == rhs.mods;

      return keyMatch && actionMatch && modsMatch;
    }
}

namespace std // OMG Barf
{
  template <>
  struct hash<dmp::Keybind>
  {
    size_t operator()(const dmp::Keybind & k) const
    {
      return k.hash();
    }
  };
}

namespace dmp
{
  static const float minZoom = 1.0f;
  static const float maxZoom = 100.0f;
  static const float zoomInc = 1.0f;
  static const float minElev = -(glm::half_pi<float>() - 0.1f);
  static const float maxElev = glm::half_pi<float>() - 0.1f;
  static const float minHorz = -std::numeric_limits<float>::infinity();
  static const float maxHorz = std::numeric_limits<float>::infinity();
  static const float rotInc = 0.2f;

  class Program
  {
  public:
    Program() = delete;
    ~Program();
    Program(const Program &) = delete;
    Program & operator=(const Program &) = delete;
    Program(Program &&) = default;
    Program & operator=(Program &&) = default;

    Program(int width, int height,
            const char * title);
    int run();
  private:
    void buildScene(TransformFn cameraFn,
                    TransformFn lightFn,
                    TransformFn quatFn,
                    TransformFn staticQuatFn);
    bool mDrawWireframe = false;
    bool mDrawNormals = false;

    RenderOptions mRenderOptions;

    float mTimeScale = 1.0f;
    Window mWindow;
    Renderer mRenderer;
    Timer mTimer;
    Scene mScene;
    std::map<std::string, float> mCameraState;
    int mLightCoeff = 0.0f;
    std::unordered_set<Keybind> mKeybinds;

    glm::quat q0;
    glm::quat q1;
    glm::quat q2;
    glm::quat q3;
    glm::quat q4;

    bool mUseCatmullRom = false;
    bool mForceShortPath = true;
    TransformFn mStaticQuatFn;
    TransformFn mQuatFn;

    size_t mSelectedQuat = 0;
    bool mIncrementQuatPos = true;

    Object * mDynamicBox = nullptr;
    bool mShowDynBox = true;

    int mMousePosX = 0;
    int mMousePosY = 0;

    typedef std::function<void(int selectedID)> OverlayCallback;
    int registerOverlayCallback(OverlayCallback cb);
    int mNextFreeOverlayID = 0;
    std::vector<OverlayCallback> mOverlayCallbacks;
  };
}


#endif
