#ifndef _MKN_GLFW_WINDOW_HPP_
#define _MKN_GLFW_WINDOW_HPP_

#include <GLFW/glfw3.h>
#include <cstdio>

namespace mkn::gl::fw {

struct WindowParams {};

static inline GLFWwindow* create_window(WindowParams const& wp = {}) {
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // To make macOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  GLFWwindow* window = glfwCreateWindow(1024, 768, "Tutorial 18 - Particles", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. "
            "Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);
  return window;
}

}  // namespace mkn::gl::fw

#endif  // _MKN_GLFW_WINDOW_HPP_
