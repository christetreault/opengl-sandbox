#include "Program.hpp"

#include <iostream>
#include <unistd.h>
#include "config.hpp"
#include "util.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <limits>
#include <boost/optional.hpp>

static const std::string HORIZONTAL = "horizontal";
static const std::string VERTICAL = "vertical";
static const std::string DISTANCE = "distance";

static void errorCb(int error, const char * description)
{
  std::cerr << "GLFW Error "
            << error
            << ": "
            << description
            << std::endl;
}

dmp::Program::Program(int width, int height,
                      const char * title)
  : mWindow(width, height, title),
    mRenderer((GLsizei) mWindow.getFramebufferWidth(),
              (GLsizei) mWindow.getFramebufferHeight()),
    mTimer()
{
  mWindow.windowSizeFn = [&](GLFWwindow * w,
                             int width,
                             int height)
    {
      int fbWidth, fbHeight;
      glfwGetFramebufferSize(w, &fbWidth, &fbHeight);
      mRenderer.resize((GLsizei) fbWidth, (GLsizei) fbHeight);
    };

  mWindow.windowFrameBufferResizeFn = [&](GLFWwindow * w,
                                          int width,
                                          int height)
    {
      mRenderer.resize((GLsizei) width, (GLsizei) height);
    };

  glfwSetErrorCallback(errorCb);

  mCameraState[HORIZONTAL] = 0.0f;
  mCameraState[VERTICAL] = 0.0f;
  mCameraState[DISTANCE] = 5.0f;

  auto cameraFn = [&mCameraState=mCameraState] (glm::mat4 &, glm::quat &, float)
    {
      boost::optional<glm::mat4> res;
      static float prevHorz = std::numeric_limits<float>::infinity();
      static float prevVert = std::numeric_limits<float>::infinity();
      static float prevDist = std::numeric_limits<float>::infinity();
      auto horz = mCameraState[HORIZONTAL];
      auto vert = mCameraState[VERTICAL];
      auto dist = mCameraState[DISTANCE];

      if (roughEq(prevHorz, horz)
          && roughEq(prevVert, vert)
          && roughEq(prevDist, dist)) return res;


      auto hRot = glm::rotate(glm::mat4(),
                              horz,
                              glm::vec3(0.0f, 1.0f, 0.0f));
      auto vRot = glm::rotate(glm::mat4(),
                              vert,
                              glm::vec3(1.0f, 0.0f, 0.0f));
      auto zoom = glm::translate(glm::mat4(),
                                 glm::vec3(0.0f, 0.0f, dist));

      res = hRot * vRot * zoom;

      prevHorz = horz;
      prevVert = vert;
      prevDist = dist;

      return res;
    };



  auto lightFn = [&l=mLightCoeff](glm::mat4 & M, glm::quat &, float deltaT)
    {
      M = glm::rotate(M,
                      ((float) l) * (deltaT / 3.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));

      return M;
    };

  auto staticQuatFn = [&](glm::mat4 &, glm::quat &, float t)
    {
      glm::quat q;
      glm::mat4 M;
      auto twoPi = 2.0f * glm::pi<float>();
      auto x = glm::mix(-4.0f, 4.0f, t);
      auto y = 0.8f * glm::cos(glm::mix(-twoPi, twoPi, t));
      auto z = 0.4f * glm::cos(glm::mix(-twoPi, twoPi, t));

      M = glm::translate(glm::mat4(), {x, y, z});


      if (t < 0.25f) q = glm::slerp(q0, q1, t * 4.0f);
      else if (t < 0.5f) q = glm::slerp(q1, q2, (t - 0.25f) * 4.0f);
      else if (t < 0.75f) q = glm::slerp(q2, q3, (t - 0.5f) * 4.0f);
      else q = glm::slerp(q3, q4, (t - 0.75f) * 4.0f);


      return M * ((glm::mat4) q);
    };

  auto quatFn = [&mTimer=mTimer,
                 &mTimeScale=mTimeScale,
                 &mForceShortPath=mForceShortPath,
                 &q0=q0, &q1=q1, &q3=q3, &q4=q4,
                 staticQuatFn](glm::mat4 &, glm::quat &, float)
    {
      boost::variant<glm::mat4> res;
      glm::quat q;
      glm::mat4 M;
      auto t = mod(mTimeScale * mTimer.time() / 5.0f, 2.0f);
      if (t > 1)
        {
          t = t - 1.0f;
          float pi = glm::pi<float>();
          float x;
          float y = 0.8f;
          float z = 0.4f - (4.0f * glm::sin(glm::mix(0.0f, pi, t)));

          float xRight = 4.0f + glm::sin(glm::mix(0.0f, pi,
                                                  glm::clamp(t, 0.0f, 0.5f) * 2.0f)) * 4.0f;
          float xLeft = -4.0f + glm::sin(glm::mix(0.0f, pi,
                                                  glm::clamp(t, 0.5f, 1.0f) * 2.0f)) * 4.0f;

          x = glm::mix(xRight, xLeft, t);

          M = glm::translate(glm::mat4(), {x, y, z});


          q = glm::slerp(q4, q0, t);

          return M * ((glm::mat4) q);
        }
      else
        {
          return staticQuatFn(M, q, t);
        }
    };

  buildScene(cameraFn, lightFn, quatFn, staticQuatFn);

  Keybind esc((GLFWwindow *) mWindow,
              [&](Keybind & k)
              {
                glfwSetWindowShouldClose(k.window, true);
              },
              GLFW_KEY_ESCAPE);
  Keybind up(mWindow,
             [&](Keybind &)
             {
               mCameraState[VERTICAL]
                 = glm::clamp(mCameraState[VERTICAL] - rotInc,
                              minElev,
                              maxElev);
             },
             GLFW_KEY_UP);
  Keybind down(mWindow,
               [&](Keybind &)
               {
                 mCameraState[VERTICAL]
                   = glm::clamp(mCameraState[VERTICAL] + rotInc,
                                minElev,
                                maxElev);
               },
               GLFW_KEY_DOWN);
  Keybind right(mWindow,
                [&](Keybind &)
                {
                  mCameraState[HORIZONTAL]
                    = glm::clamp(mCameraState[HORIZONTAL] + rotInc,
                                 minHorz,
                                 maxHorz);
                },
                GLFW_KEY_RIGHT);
  Keybind left(mWindow,
               [&](Keybind &)
               {
                 mCameraState[HORIZONTAL]
                   = glm::clamp(mCameraState[HORIZONTAL] - rotInc,
                                minHorz,
                                maxHorz);
               },
               GLFW_KEY_LEFT);

  Keybind pageUp(mWindow,
                 [&](Keybind &)
                 {
                   mCameraState[DISTANCE]
                     = glm::clamp(mCameraState[DISTANCE] - zoomInc,
                                  minZoom,
                                  maxZoom);
                 },
                 GLFW_KEY_PAGE_UP);
  Keybind pageDown(mWindow,
                   [&](Keybind &)
                   {
                     mCameraState[DISTANCE]
                       = glm::clamp(mCameraState[DISTANCE] + zoomInc,
                                    minZoom,
                                    maxZoom);
                   },
                   GLFW_KEY_PAGE_DOWN);
  Keybind w(mWindow,
            [&](Keybind &)
            {
              mRenderOptions.drawWireframe = !(mRenderOptions.drawWireframe);
            },
            GLFW_KEY_W);
  Keybind n(mWindow,
            [&](Keybind &)
            {
              mRenderOptions.drawNormals = !(mRenderOptions.drawNormals);
            },
            GLFW_KEY_N);
  Keybind comma(mWindow,
                [&](Keybind &)
                {
                  if (mTimeScale < 0.1f) return;
                  else if (mTimeScale < 2.0f) mTimeScale = mTimeScale - 0.1f;
                  else mTimeScale = mTimeScale - 1.0f;
                },
                GLFW_KEY_COMMA);
  Keybind period(mWindow,
                 [&](Keybind &)
                 {
                   if (mTimeScale > 10.0f) return;
                   else if (mTimeScale > 0.9f) mTimeScale = mTimeScale + 1.0f;
                   else mTimeScale = mTimeScale + 0.1f;
                 },
                 GLFW_KEY_PERIOD);
  Keybind l(mWindow,
            [&](Keybind &)
            {
              if (mLightCoeff == 0) mLightCoeff = 1;
              else if (mLightCoeff == 1) mLightCoeff = -1;
              else mLightCoeff = 0;
            },
            GLFW_KEY_L);

  Keybind one(mWindow,
               [&](Keybind &)
               {
                 mSelectedQuat = 0;
               },
               GLFW_KEY_1);

  Keybind two(mWindow,
              [&](Keybind &)
              {
                mSelectedQuat = 1;
              },
              GLFW_KEY_2);

  Keybind three(mWindow,
              [&](Keybind &)
              {
                mSelectedQuat = 2;
              },
              GLFW_KEY_3);

  Keybind four(mWindow,
                [&](Keybind &)
                {
                  mSelectedQuat = 3;
                },
                GLFW_KEY_4);

  Keybind five(mWindow,
               [&](Keybind &)
               {
                 mSelectedQuat = 4;
               },
               GLFW_KEY_5);

  Keybind i(mWindow,
               [&](Keybind &)
               {
                 float delta = mIncrementQuatPos ? 0.1f : -0.1f;
                 glm::quat q = glm::rotate(glm::quat(),
                                           delta, {1.0f, 0.0f, 0.0f});
                 switch (mSelectedQuat)
                   {
                   case 0:
                     q0 = q0 * q;
                     return;
                   case 1:
                     q1 = q1 * q;
                     return;
                   case 2:
                     q2 = q2 * q;
                     return;
                   case 3:
                     q3 = q3 * q;
                     return;
                   case 4:
                     q4 = q4 * q;
                     return;
                   default:
                     return;
                   }
               },
               GLFW_KEY_I);

  Keybind j(mWindow,
               [&](Keybind &)
               {
                 float delta = mIncrementQuatPos ? 0.1f : -0.1f;
                 glm::quat q = glm::rotate(glm::quat(),
                                           delta, {0.0f, 1.0f, 0.0f});
                 switch (mSelectedQuat)
                   {
                   case 0:
                     q0 = q0 * q;
                     return;
                   case 1:
                     q1 = q1 * q;
                     return;
                   case 2:
                     q2 = q2 * q;
                     return;
                   case 3:
                     q3 = q3 * q;
                     return;
                   case 4:
                     q4 = q4 * q;
                     return;
                   default:
                     return;
                   }
               },
               GLFW_KEY_J);

  Keybind k(mWindow,
               [&](Keybind &)
               {
                 float delta = mIncrementQuatPos ? 0.1f : -0.1f;
                 glm::quat q = glm::rotate(glm::quat(),
                                           delta, {0.0f, 0.0f, 1.0f});
                 switch (mSelectedQuat)
                   {
                   case 0:
                     q0 = q0 * q;
                     return;
                   case 1:
                     q1 = q1 * q;
                     return;
                   case 2:
                     q2 = q2 * q;
                     return;
                   case 3:
                     q3 = q3 * q;
                     return;
                   case 4:
                     q4 = q4 * q;
                     return;
                   default:
                     return;
                   }
               },
               GLFW_KEY_K);

  Keybind tab(mWindow,
               [&](Keybind &)
               {
                 mIncrementQuatPos = !mIncrementQuatPos;
               },
               GLFW_KEY_TAB);

  Keybind s(mWindow,
               [&](Keybind &)
               {
                 mRenderOptions.drawOverlays = !(mRenderOptions.drawOverlays);
               },
               GLFW_KEY_S);

  mKeybinds = {esc, up, down, right, left, pageUp, pageDown,
               w, n, l, comma, period, one, two, three, four, five,
               i, j, k, tab, s};

  mWindow.keyFn = [&mKeybinds=mKeybinds](GLFWwindow * w,
                                         int key,
                                         int scancode,
                                         int action,
                                         int mods)
    {
      expect("keybinds not empty", !mKeybinds.empty());
      Keybind got = {};
      got.window = w;
      got.key = key;
      got.scancode = scancode;
      got.action = action;
      got.mods = mods;

      auto res = mKeybinds.find(got);

      if (res != mKeybinds.end())
        {
          res->fn(got);
        }
    };

  mWindow.cursorPosFn = [&mMousePosX=mMousePosX,
                         &mMousePosY=mMousePosY](GLFWwindow *,
                                                 double x, double y)
    {
      mMousePosX = static_cast<int>(x);
      mMousePosY = static_cast<int>(y);
    };

  mWindow.mouseButtonFn = [&mRenderer=mRenderer,
                           &mScene=mScene,
                           &mRenderOptions=mRenderOptions,
                           &mOverlayCallbacks=mOverlayCallbacks,
                           &mMousePosX=mMousePosX,
                           &mMousePosY=mMousePosY](GLFWwindow *,
                                                   int button,
                                                   int action,
                                                   int mods)
    {
      if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
          if (action == GLFW_RELEASE)
            {
              int id = mRenderer.pick(mScene,
                                      mRenderOptions,
                                      mMousePosX,
                                      mMousePosY);
              if (id < 255) mOverlayCallbacks[id](id);
            }
        }
    };
}

static void updateFPS(dmp::Window & window, const dmp::Timer & timer, float scale)
{
  static size_t fps = 0;
  static float timeElapsed = 0.0f;
  float runTime = timer.time();

  ++fps;

  if ((runTime - timeElapsed) >= 1.0f)
    {
      window.updateFPS(fps, 1000/fps, scale);

      fps = 0;
      timeElapsed += 1.0f;
    }
}

int dmp::Program::run()
{
  mTimer.reset();
  mTimer.unpause();

  while (!mWindow.shouldClose())
    {
      // time marches on...
      mTimer.tick();

      updateFPS(mWindow, mTimer, mTimeScale);

      // do actual work

      if (mTimer.isPaused())
        {
          sleep(100);
        }
      else
        {
          mScene.update(mTimer.deltaTime() * mTimeScale);
          mRenderer.render(mScene, mTimer, mRenderOptions);
          mWindow.swapBuffer();
        }

      // poll window system events

      mWindow.pollEvents();
    }
  return EXIT_SUCCESS;
}

dmp::Program::~Program()
{
  mScene.free();
}

void dmp::Program::buildScene(TransformFn cameraFn,
                              TransformFn lightFn,
                              TransformFn quatFn,
                              TransformFn staticQuatFn)
{
  mScene.graph = std::make_unique<Branch>();

  std::string notex = "";
  mScene.textures.emplace_back(notex);
  std::string someRandomTexture = skyBox[0];
  mScene.textures.emplace_back(someRandomTexture);

  mScene.materials.push_back( // Ruby = 0
    {
      {
        0.1745f, 0.01175f, 0.01175f, 1.0f
      },
      {
        0.61424f, 0.04136f, 0.04136f, 1.0f
      },
      {
        0.727811f, 0.626959f, 0.626959f, 1.0f
      },
      0.6f
    });
  mScene.materials.push_back( // Pearl = 1
    {
      {
        0.25f, 0.20725f, 0.20725f, 1.0f
      },
      {
        1.0f, 0.829f, 0.829f, 1.0f
      },
      {
        0.296648, 0.296648, 0.296648, 1.0f
      },
      0.6f
    });

  mScene.materialConstants =
    std::make_unique<UniformBuffer>(mScene.materials.size(),
                                    Material::std140Size());

  for (size_t i = 0; i < mScene.materials.size(); ++i)
    {
      mScene.materialConstants->update(i, mScene.materials[i]);
    }

  mScene.lights.push_back({
       {0.05f, 1.0f, 0.05f, 1.0f},
       {0.0f, 0.9f, 1.0f, 0.0f},
       glm::mat4()
     });
   mScene.lights.push_back({
       {1.0f, 0.05f, 0.05f, 1.0f},
       {0.0f, 0.0f, 1.0f, 0.0f},
       glm::mat4()
     });
   mScene.lights.push_back({
       {0.05f, 0.05f, 1.0f, 1.0f},
       {0.0f, -0.9f, 1.0f, 0.0f},
       glm::mat4()
     });
   mScene.lights.push_back({
       {0.3f, 0.3f, 0.3f, 1.0f},
       {-1.0f, 0.0f, 0.0f, 0.0f},
       glm::mat4()
     });

   auto lightRot = mScene.graph->transform(lightFn);
   auto lightGroup = lightRot->branch();

   auto redLightRotation = glm::rotate(glm::mat4(),
                                       glm::pi<float>() / 2.0f,
                                       glm::vec3(0.0f, 1.0f, 0.0f));
   auto redLight = lightGroup->transform(redLightRotation);
   auto greenLightRotation = glm::rotate(glm::mat4(),
                                         (7.0f * glm::pi<float>()) / 6.0f,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
   auto greenLight = lightGroup->transform(greenLightRotation);
   auto blueLightRotation = glm::rotate(glm::mat4(),
                                        (11.0f * glm::pi<float>()) / 6.0f,
                                        glm::vec3(0.0f, 1.0f, 0.0f));
   auto blueLight = lightGroup->transform(blueLightRotation);
   auto cam = mScene.graph->transform(cameraFn);
   redLight->insert(mScene.lights[1]);
   greenLight->insert(mScene.lights[0]);
   blueLight->insert(mScene.lights[2]);
   mScene.graph->insert(mScene.lights[3]);

   mScene.cameras.emplace_back();
   mScene.graph->insert(mScene.cameras[0].focus());
   cam->insert(mScene.cameras[0].pos());

   glm::vec4 max(0.2f, 0.5f, 0.1f, 1.0f);
   glm::vec4 min = -max;

   Object build1(Cube, min, max, 1, 0);
   auto staticBoxOne = [=](glm::mat4 & M, glm::quat & q, float)
     {
       return staticQuatFn(M, q, 0.0f);
     };

   auto boxOne = mScene.graph->transform(staticBoxOne);
   mScene.objects.push_back(boxOne->insert(build1));

   Object build2(Cube, min, max, 1, 0);
   auto staticBoxTwo = [=](glm::mat4 & M, glm::quat & q, float)
     {
       return staticQuatFn(M, q, 0.25f);
     };

   auto boxTwo = mScene.graph->transform(staticBoxTwo);
   mScene.objects.push_back(boxTwo->insert(build2));

   Object build3(Cube, min, max, 1, 0);
   auto staticBoxThree = [=](glm::mat4 & M, glm::quat & q, float)
     {
       return staticQuatFn(M, q, 0.5f);
     };

   auto boxThree = mScene.graph->transform(staticBoxThree);
   mScene.objects.push_back(boxThree->insert(build3));

   Object build4(Cube, min, max, 1, 0);
   auto staticBoxFour = [=](glm::mat4 & M, glm::quat & q, float)
     {
       return staticQuatFn(M, q, 0.75f);
     };

   auto boxFour = mScene.graph->transform(staticBoxFour);
   mScene.objects.push_back(boxFour->insert(build4));

   Object build5(Cube, min, max, 1, 0);
   auto staticBoxFive = [=](glm::mat4 & M, glm::quat & q, float)
     {
       return staticQuatFn(M, q, 1.0f);
     };

   auto boxFive = mScene.graph->transform(staticBoxFive);
   mScene.objects.push_back(boxFive->insert(build5));

   Object buildLerp(Cube, min, max, 0, 0);
   auto lerpBox = mScene.graph->transform(quatFn);
   mDynamicBox = lerpBox->insert(buildLerp);
   mScene.objects.push_back(mDynamicBox);

   mScene.objectConstants
     = std::make_unique<UniformBuffer>(mScene.objects.size(),
                                       ObjectConstants::std140Size());

  std::vector<const char *> sb;
  for (size_t i = 0; i < 6; ++i) sb.push_back(skyBox[i]);

  mScene.skybox = std::make_unique<Skybox>(sb);

  mScene.overlays.emplace_back(-0.75f, -0.8f,
                               1.5f, 0.2f,
                               registerOverlayCallback([](int selectedID){std::cerr << "Callback 0: selected: " << selectedID << std::endl;}),
                               mScene.textures[1]);

  mScene.overlays.emplace_back(-0.5f, -0.75f,
                               1.0f, 0.2f,
                               //registerOverlayCallback([](int selectedID){std::cerr << "Callback 1: selected: " << selectedID << std::endl;}),
                               mScene.textures[1]);

  mScene.overlays.emplace_back(-0.25f, -0.7f,
                               0.5f, 0.2f,
                               registerOverlayCallback([](int selectedID){std::cerr << "Callback 2: selected: " << selectedID << std::endl;}),
                               mScene.textures[1]);

  mScene.overlayConstants
    = std::make_unique<UniformBuffer>(mScene.overlays.size(),
                                      OverlayConstants::std140Size());

  Object::sortByMaterial(mScene.objects);
  mScene.graph->update(0.0f, glm::mat4(), true);
}

int dmp::Program::registerOverlayCallback(OverlayCallback cb)
{
  expect("Less than 255 overlay callbacks", mNextFreeOverlayID < 255);
  auto retval = mNextFreeOverlayID;
  ++mNextFreeOverlayID;
  mOverlayCallbacks.push_back(cb);
  return retval;
}
