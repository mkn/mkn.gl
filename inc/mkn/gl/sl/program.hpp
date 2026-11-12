#ifndef _MKN_GL_SL_PROGRAM_HPP_
#define _MKN_GL_SL_PROGRAM_HPP_

#include <GL/glew.h>

#include <string>
#include <utility>

#include "mkn/gl/shaders.hpp"

namespace mkn::gl::sl {

// owns a linked shader program
struct Program {
  Program() = default;
  explicit Program(GLuint const i) : id{i} {}
  Program(Program&& that) noexcept : id{std::exchange(that.id, 0)} {}
  Program& operator=(Program&& that) noexcept {
    std::swap(id, that.id);
    return *this;
  }
  Program(Program const&) = delete;
  Program& operator=(Program const&) = delete;
  ~Program() {
    if (id) glDeleteProgram(id);
  }

  void use() const { glUseProgram(id); }
  GLint uniform(char const* const name) const { return glGetUniformLocation(id, name); }

  // from shader sources
  static Program from_source(std::string const& vertex, std::string const& fragment) {
    return Program{mkn::gl::shaders::program(vertex, fragment)};
  }

  GLuint id = 0;
};

// from shader files
struct VertexFragmentProgram : Program {
  VertexFragmentProgram(std::string const& vfp, std::string const& ffp)
      : Program{mkn::gl::shaders::load(vfp, ffp)}, vertex_file_path{vfp}, fragment_file_path{ffp} {}

  std::string const vertex_file_path;
  std::string const fragment_file_path;
};

}  // namespace mkn::gl::sl

#endif  // _MKN_GL_SL_PROGRAM_HPP_
