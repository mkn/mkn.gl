#ifndef _MKN_GL_SHADERS_HPP_
#define _MKN_GL_SHADERS_HPP_

#include <GL/glew.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <optional>

namespace mkn::gl::shaders {

std::optional<std::string> file_to_string(auto const &file_path) {
  std::string file_contents;
  std::ifstream ifs(file_path, std::ios::in);
  if (ifs.is_open()) {
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    file_contents = sstr.str();
    ifs.close();
    return file_contents;
  }
  printf(
      "Impossible to open %s. Are you in the right directory ? Don't "
      "forget to read the FAQ !\n",
      file_path);
  getchar();

  return std::nullopt;
}

auto static inline load(char const *vertex_file_path, char const *fragment_file_path) {
  decltype(glCreateProgram()) ret = 0;
  // Create the shaders
  auto VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  auto FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  auto optional_vtx = file_to_string(vertex_file_path);
  if (!optional_vtx) return ret;
  auto VertexShaderCode = *optional_vtx;

  auto optional_fragement = file_to_string(fragment_file_path);
  auto FragmentShaderCode = optional_fragement ? *optional_fragement : "";

  auto Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const *VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }

  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const *FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }

  // Link the program
  printf("Linking program\n");
  auto ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

auto static inline load(std::string const &vfp, std::string const &ffp) {
  return load(vfp.c_str(), ffp.c_str());
}

}  // namespace mkn::gl::shaders

#endif  //_MKN_GL_SHADERS_HPP_
