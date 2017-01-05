#ifndef DMP_SHADER_HPP
#define DMP_SHADER_HPP

#include <vector>
#include <string>
#include <map>
#include <GL/glew.h>
#include "../util.hpp"

namespace dmp
{
  class Shader
  {
  public:
    Shader() {}
    Shader(const char * vertPath,
           const char * geomPath,
           const char * tescPath,
           const char * tesePath,
           const char * fragPath);

    operator GLuint() const
    {
      expect("Cast invalid Shader to GLuint!",
             mShaderProg != 0);
      return mShaderProg;
    }

    ~Shader()
    {
      if (mShaderProg != 0)
        {
          glDeleteProgram(mShaderProg);
        }
    }

    void initShader(const char * vertPath,
                    const char * geomPath,
                    const char * tescPath,
                    const char * tesePath,
                    const char * fragPath);
  private:
    static std::map<const std::string, std::vector<char>> memo;
    static std::vector<char> loadGLSL(const std::string & path);
    GLuint mShaderProg = 0;
  };
}


#endif
