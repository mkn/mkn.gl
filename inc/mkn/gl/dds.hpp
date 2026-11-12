#ifndef _MKN_GL_DDS_HPP_
#define _MKN_GL_DDS_HPP_

#include <GL/glew.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mkn::gl::dds {

// loads a DXT1/3/5 compressed DDS file with its mipmaps into a new 2D texture,
// throws if the file can't be read or isn't a supported DDS
GLuint inline static load(std::string const& imagepath) {
  auto constexpr static FOURCC_DXT1 = 0x31545844;  // "DXT1" in ASCII
  auto constexpr static FOURCC_DXT3 = 0x33545844;  // "DXT3" in ASCII
  auto constexpr static FOURCC_DXT5 = 0x35545844;  // "DXT5" in ASCII

  std::ifstream file(imagepath, std::ios::binary);
  if (!file) throw std::runtime_error("cannot open DDS file: " + imagepath);

  char filecode[4];
  unsigned char header[124];
  if (!file.read(filecode, 4) || std::strncmp(filecode, "DDS ", 4) != 0 ||
      !file.read(reinterpret_cast<char*>(header), sizeof(header)))
    throw std::runtime_error("not a DDS file: " + imagepath);

  auto const u32 = [&](std::size_t const at) {
    std::uint32_t v;
    std::memcpy(&v, header + at, sizeof(v));
    return v;
  };
  auto height = u32(8), width = u32(12);
  auto const linearSize = u32(16), mipMapCount = u32(24), fourCC = u32(80);

  GLenum format;
  switch (fourCC) {
    case FOURCC_DXT1:
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      break;
    case FOURCC_DXT3:
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      break;
    case FOURCC_DXT5:
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
    default:
      throw std::runtime_error("unsupported DDS format, DXT1/3/5 only: " + imagepath);
  }

  // all mipmaps
  std::vector<unsigned char> buffer(mipMapCount > 1 ? linearSize * 2 : linearSize);
  file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(buffer.size()));

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  std::uint32_t const blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  std::size_t offset = 0;
  for (std::uint32_t level = 0; level < mipMapCount && (width || height); ++level) {
    std::uint32_t const size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
    if (offset + size > buffer.size()) break;  // truncated file
    glCompressedTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(level), format,
                           static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0,
                           static_cast<GLsizei>(size), buffer.data() + offset);
    offset += size;
    width = width > 1 ? width / 2 : 1;  // non power of two textures
    height = height > 1 ? height / 2 : 1;
  }
  return textureID;
}

}  // namespace mkn::gl::dds

#endif  // _MKN_GL_DDS_HPP_
