#ifndef _MKN_GLFW_WINDOW_HPP_
#define _MKN_GLFW_WINDOW_HPP_

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

namespace mkn::gl::fw {

struct WindowParams {
  int width = 1024, height = 768;
  std::string title = "mkn.gl";
  int gl_major = 3, gl_minor = 3;
  bool core = true;  // core profile, forward compatible as macOS requires
  bool resizable = true;
  bool visible = true;
  int samples = 0;        // multisampling, 0 for none
  int swap_interval = 1;  // 1 waits for vsync, 0 doesn't
};

// creates a window and makes its context current, throws if the context can't be created
static inline GLFWwindow* create_window(WindowParams const& wp = {}) {
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, wp.gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, wp.gl_minor);
  if (wp.core) {
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  glfwWindowHint(GLFW_RESIZABLE, wp.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, wp.visible ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_SAMPLES, wp.samples);

  auto* const window = glfwCreateWindow(wp.width, wp.height, wp.title.c_str(), nullptr, nullptr);
  if (!window)
    throw std::runtime_error("glfwCreateWindow failed, is OpenGL " + std::to_string(wp.gl_major) +
                             "." + std::to_string(wp.gl_minor) + " available?");
  glfwMakeContextCurrent(window);
  glfwSwapInterval(wp.swap_interval);
  return window;
}

}  // namespace mkn::gl::fw

#endif  // _MKN_GLFW_WINDOW_HPP_
