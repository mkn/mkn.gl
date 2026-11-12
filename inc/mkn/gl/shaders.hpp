#ifndef _MKN_GL_SHADERS_HPP_
#define _MKN_GL_SHADERS_HPP_

#include <GL/glew.h>

#include <fstream>
#include <initializer_list>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

namespace mkn::gl::shaders {

std::optional<std::string> static inline file_to_string(std::string const& file_path) {
  std::ifstream ifs(file_path, std::ios::in);
  if (!ifs.is_open()) return std::nullopt;
  std::stringstream sstr;
  sstr << ifs.rdbuf();
  return sstr.str();
}

// compiles one shader, throws with the compile log on failure
GLuint static inline compile(GLenum const type, std::string const& source) {
  auto const shader = glCreateShader(type);
  char const* src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);
  GLint ok = GL_FALSE, length = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (ok) return shader;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  std::string log(static_cast<std::size_t>(length > 0 ? length : 1), '\0');
  glGetShaderInfoLog(shader, length, nullptr, log.data());
  glDeleteShader(shader);
  throw std::runtime_error("shader compile failed: " + log);
}

// links compiled shaders into a program and deletes the shaders, throws with the link log
GLuint static inline link(std::initializer_list<GLuint> const shaders) {
  auto const program = glCreateProgram();
  for (auto const s : shaders) glAttachShader(program, s);
  glLinkProgram(program);
  for (auto const s : shaders) {
    glDetachShader(program, s);
    glDeleteShader(s);
  }
  GLint ok = GL_FALSE, length = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &ok);
  if (ok) return program;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  std::string log(static_cast<std::size_t>(length > 0 ? length : 1), '\0');
  glGetProgramInfoLog(program, length, nullptr, log.data());
  glDeleteProgram(program);
  throw std::runtime_error("shader link failed: " + log);
}

// program from vertex and fragment shader sources
GLuint static inline program(std::string const& vertex, std::string const& fragment) {
  auto const vs = compile(GL_VERTEX_SHADER, vertex);
  GLuint fs = 0;
  try {
    fs = compile(GL_FRAGMENT_SHADER, fragment);
  } catch (...) {
    glDeleteShader(vs);
    throw;
  }
  return link({vs, fs});
}

// program from vertex and fragment shader files
GLuint static inline load(std::string const& vertex_file_path,
                          std::string const& fragment_file_path) {
  auto const read = [](std::string const& path) {
    auto const s = file_to_string(path);
    if (!s) throw std::runtime_error("cannot read shader file: " + path);
    return *s;
  };
  return program(read(vertex_file_path), read(fragment_file_path));
}

}  // namespace mkn::gl::shaders

#endif  // _MKN_GL_SHADERS_HPP_
