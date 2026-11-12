#ifndef _MKN_GL_EX_VIEW_HPP_
#define _MKN_GL_EX_VIEW_HPP_

// First person camera: mouse looks, WASD or arrows move.

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace example {

struct Viewer {
  explicit Viewer(GLFWwindow* const w) : window{w} {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    lastTime = glfwGetTime();
  }

  // updates the view and projection from input since the last tick
  void tick() {
    double const now = glfwGetTime();
    auto const dt = static_cast<float>(now - lastTime);
    lastTime = now;

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (first) {  // disabling the cursor moves it, ignore that
      lastX = x;
      lastY = y;
      first = false;
    }
    horizontalAngle += mouseSpeed * static_cast<float>(lastX - x);
    verticalAngle += mouseSpeed * static_cast<float>(lastY - y);
    lastX = x;
    lastY = y;

    glm::vec3 const direction(std::cos(verticalAngle) * std::sin(horizontalAngle),
                              std::sin(verticalAngle),
                              std::cos(verticalAngle) * std::cos(horizontalAngle));
    glm::vec3 const right(std::sin(horizontalAngle - 3.14f / 2.0f), 0,
                          std::cos(horizontalAngle - 3.14f / 2.0f));
    glm::vec3 const up = glm::cross(right, direction);

    auto const key = [&](int a, int b) {
      return glfwGetKey(window, a) == GLFW_PRESS || glfwGetKey(window, b) == GLFW_PRESS;
    };
    if (key(GLFW_KEY_UP, GLFW_KEY_W)) position += direction * dt * speed;
    if (key(GLFW_KEY_DOWN, GLFW_KEY_S)) position -= direction * dt * speed;
    if (key(GLFW_KEY_RIGHT, GLFW_KEY_D)) position += right * dt * speed;
    if (key(GLFW_KEY_LEFT, GLFW_KEY_A)) position -= right * dt * speed;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float const aspect = height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.f;
    proj_mat = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
    view_mat = glm::lookAt(position, position + direction, up);
  }

  GLFWwindow* window;
  glm::mat4 view_mat{1}, proj_mat{1};

  glm::vec3 position{0, 0, 5};  // on +Z
  float horizontalAngle = 3.14f;  // toward -Z
  float verticalAngle = 0.0f;
  float fov = 45.0f;
  float speed = 3.0f;  // units / second
  float mouseSpeed = 0.005f;

 private:
  double lastX = 0, lastY = 0, lastTime = 0;
  bool first = true;
};

}  // namespace example

#endif  // _MKN_GL_EX_VIEW_HPP_
