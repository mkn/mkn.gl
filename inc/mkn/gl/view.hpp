#ifndef _MKN_GL_VIEW_HPP_
#define _MKN_GL_VIEW_HPP_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <optional>

namespace mkn::gl {

struct Viewer {
  Viewer(GLFWwindow* w) : window{w} { glfwSetScrollCallback(window, scroll_callback); }

  void tick();
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

  GLFWwindow* window;
  glm::mat4 view_mat;
  glm::mat4 proj_mat;

  // Initial position : on +Z
  glm::vec3 position = glm::vec3(0, 0, 5);
  // Initial horizontal angle : toward -Z
  float horizontalAngle = 3.14f;
  // Initial vertical angle : none
  float verticalAngle = 0.0f;
  // Initial Field of View
  float initialFoV = 45.0f;

  float speed = 3.0f;  // 3 units / second
  float mouseSpeed = 0.005f;
};

void Viewer::tick() {
  // glfwGetTime is called only once, the first time this function is called
  static double lastTime = glfwGetTime();

  // Compute time difference between current and last frame
  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);

  // Get mouse position
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  // Reset mouse position for next frame
  glfwSetCursorPos(window, 1024.f / 2, 768.f / 2);

  // Compute new orientation
  horizontalAngle += mouseSpeed * float(1024.f / 2 - xpos);
  verticalAngle += mouseSpeed * float(768.f / 2 - ypos);

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle),
                      cos(verticalAngle) * cos(horizontalAngle));

  // Right vector
  glm::vec3 right =
      glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));

  // Up vector
  glm::vec3 up = glm::cross(right, direction);

  // Move forward
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += direction * deltaTime * speed;
  }
  // Move backward
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= direction * deltaTime * speed;
  }
  // Strafe right
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += right * deltaTime * speed;
  }
  // Strafe left
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= right * deltaTime * speed;
  }

  float FoV = initialFoV;  // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a
                           // callback for this. It's a bit too complicated for this beginner's
                           // tutorial, so it's disabled instead.

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  proj_mat = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  view_mat =
      glm::lookAt(position,              // Camera is here
                  position + direction,  // and looks here : at the same position, plus "direction"
                  up                     // Head is up (set to 0,-1,0 to look upside-down)
      );

  // For the next frame, the "last time" will be "now"
  lastTime = currentTime;
}

}  // namespace mkn::gl

#endif  // _MKN_GL_VIEW_HPP_
