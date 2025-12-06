#ifndef _MKN_GL_FW_HPP_
#define _MKN_GL_FW_HPP_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <optional>

#include "mkn/kul/except.hpp"

namespace mkn::gl::fw {

auto static inline init() {
  if (!glfwInit()) throw std::runtime_error("glfwInit() failed!");
}

}  // namespace mkn::gl::fw

#endif  // _MKN_GL_FW_HPP_
