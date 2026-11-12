#ifndef _MKN_GL_EW_HPP_
#define _MKN_GL_EW_HPP_

#include <GL/glew.h>

#include <stdexcept>
#include <string>

namespace mkn::gl::ew {

// loads OpenGL functions for the current context, see fw::create_window
auto static inline init() {
  glewExperimental = GL_TRUE;  // needed for core profiles
  if (auto const e = glewInit(); e != GLEW_OK)
    throw std::runtime_error(std::string("glewInit failed: ") +
                             reinterpret_cast<char const*>(glewGetErrorString(e)));
  glGetError();  // glewInit can leave GL_INVALID_ENUM on core profiles
}

}  // namespace mkn::gl::ew

#endif  // _MKN_GL_EW_HPP_
