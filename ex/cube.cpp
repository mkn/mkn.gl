// A coloured cube with a first person camera, Escape quits.
//   cube [--frames N]   exit after N frames, e.g. for CI

#include "mkn/gl.hpp"
#include "view.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cstdlib>
#include <iostream>
#include <string_view>

namespace {

char const* const VERTEX = R"(#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 mvp;
out vec3 colour;
void main() {
  colour = position * 0.5 + 0.5;
  gl_Position = mvp * vec4(position, 1.0);
}
)";

char const* const FRAGMENT = R"(#version 330 core
in vec3 colour;
out vec4 fragment;
void main() { fragment = vec4(colour, 1.0); }
)";

std::array<GLfloat, 24> constexpr CORNERS{-1, -1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1,
                                         -1, -1, 1,  1, -1, 1,  1, 1, 1,  -1, 1, 1};
std::array<GLuint, 36> constexpr TRIANGLES{0, 1, 2, 2, 3, 0, 4, 6, 5, 6, 4, 7,
                                           0, 4, 5, 5, 1, 0, 3, 2, 6, 6, 7, 3,
                                           0, 3, 7, 7, 4, 0, 1, 5, 6, 6, 2, 1};

}  // namespace

int main(int argc, char* argv[]) {
  long frames = -1;
  if (argc > 2 && std::string_view(argv[1]) == "--frames") frames = std::atol(argv[2]);

  try {
    mkn::gl::fw::init();
    auto* const window = mkn::gl::fw::create_window({.title = "mkn.gl cube", .samples = 4});
    mkn::gl::ew::init();

    auto const program = mkn::gl::sl::Program::from_source(VERTEX, FRAGMENT);
    auto const mvp = program.uniform("mvp");

    GLuint vao, buffers[2];
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CORNERS), CORNERS.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TRIANGLES), TRIANGLES.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);

    example::Viewer viewer(window);
    for (long frame = 0; frame != frames && !glfwWindowShouldClose(window); ++frame) {
      glfwPollEvents();
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
      viewer.tick();

      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      glViewport(0, 0, width, height);
      glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      auto const model = glm::rotate(glm::mat4(1), static_cast<float>(glfwGetTime()),
                                     glm::vec3(0.3f, 1.0f, 0.0f));
      auto const m = viewer.proj_mat * viewer.view_mat * model;
      program.use();
      glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m));
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(TRIANGLES.size()), GL_UNSIGNED_INT,
                     nullptr);
      glfwSwapBuffers(window);
    }

    glDeleteBuffers(2, buffers);
    glDeleteVertexArrays(1, &vao);
    if (auto const e = glGetError(); e != GL_NO_ERROR) {
      std::cerr << "OpenGL error: " << e << std::endl;
      return 1;
    }
  } catch (std::exception const& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  glfwTerminate();
  return 0;
}
