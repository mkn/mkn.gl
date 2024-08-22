#ifndef _MKN_GL_SL_PROGRAM_HPP_
#define _MKN_GL_SL_PROGRAM_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>

#include "mkn/gl/shaders.hpp"

namespace mkn::gl::sl {

struct Program {
  void tick() { glUseProgram(id); }
  ~Program() { glDeleteProgram(id); }

  GLuint id = 0;
};

struct VertexFragmentProgram : Program {
  //

  //
  VertexFragmentProgram(std::string const& vfp, std::string const& ffp)
      : Program{mkn::gl::shaders::load(vfp, ffp)}, vertex_file_path{vfp}, fragment_file_path{ffp} {}

  std::string const vertex_file_path;
  std::string const fragment_file_path;
};

}  // namespace mkn::gl::sl

#endif  // _MKN_GL_SL_PROGRAM_HPP_
