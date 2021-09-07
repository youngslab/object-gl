#include "object/gl.hpp"
#include <GLES3/gl3.h>
#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gl {

Shader::Shader(GLuint shader)
    : raii::AutoDeletable<GLuint>(shader,
				  [](auto shader) { glDeleteShader(shader); }) {
}

Program::Program(GLuint program)
    : raii::AutoDeletable<GLuint>(program, [](auto p) { glDeleteProgram(p); }) {
}

VertexArray::VertexArray(GLuint vao)
    : raii::AutoDeletable<GLuint>(
	  vao, [](auto vao) { glDeleteVertexArrays(1, &vao); }) {}

Buffer::Buffer(GLuint buffer)
    : raii::AutoDeletable<GLuint>(
	  buffer, [](auto buffer) { glDeleteBuffers(1, &buffer); }) {}

Texture::Texture(GLuint texture)
    : raii::AutoDeletable<GLuint>(
	  texture, [](auto texture) { glDeleteBuffers(1, &texture); }) {}

// create programs
auto createProgram() -> Program { return Program(glCreateProgram()); }

auto createShader(GLenum type) -> Shader {
  return Shader(glCreateShader(type));
}

auto createProgram_(std::string const &vertex, std::string const &fragment)
    -> Program {

  auto vinterm = vertex.c_str();
  auto vert = gl::createShader_(GL_VERTEX_SHADER, 1, &vinterm, nullptr);

  auto finterm = fragment.c_str();
  auto frag = gl::createShader_(GL_FRAGMENT_SHADER, 1, &finterm, nullptr);

  return gl::createProgram_(vert, frag);
}

auto createProgram_(GLuint vertexShader, GLuint fragmentShader) -> Program {
  auto program = gl::createProgram();
  glAttachShader(program, fragmentShader);
  glAttachShader(program, vertexShader);
  glLinkProgram(program);

  GLint linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (linked == 0) {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program, 1000, &len, log);
    throw std::runtime_error(
	fmt::format("Failed to link program. program={}, "
		    "vertexShader={}, fragmentShader={}. glError={} \n{}",
		    program, vertexShader, fragmentShader, glGetError(), log));
  }
  return Program(program);
}

// utilities
auto createShader_(GLenum type, GLsizei count, const GLchar *const *string,
		   const GLint *length) -> Shader {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, count, string, length);
  glCompileShader(shader);

  GLint compiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (compiled == 0) {
    char log[1000];
    GLsizei len;
    glGetShaderInfoLog(shader, 1000, &len, log);
    throw std::runtime_error(fmt::format(
	"ERROR: Failed to compile shader code. glError={}, log=\"{}\" \n{}",
	glGetError(), log, string[0]));
  }
  return Shader(shader);
}

// Gen-Functions
auto genVertexArray() -> VertexArray {
  GLuint vao;
  glGenVertexArrays(1, &vao);
  return VertexArray(vao);
}

auto genVertexArrays(GLsizei n) -> std::vector<VertexArray> {
  std::vector<GLuint> vaos(n);
  glGenVertexArrays(n, vaos.data());

  std::vector<VertexArray> res;
  std::transform(vaos.begin(), vaos.end(), res.begin(),
		 [](auto vao) { return VertexArray(vao); });

  return res;
}

auto genBuffer() -> Buffer {
  GLuint bo;
  glGenBuffers(1, &bo);
  return Buffer(bo);
}

auto genBuffers(GLsizei n) -> std::vector<Buffer> {
  std::vector<GLuint> bos(n);
  glGenBuffers(n, bos.data());

  std::vector<Buffer> res;
  std::transform(bos.begin(), bos.end(), res.begin(),
		 [](auto bo) { return Buffer(bo); });

  return res;
}

auto genTexture() -> Texture {
  GLuint texture = 0;
  glGenTextures(1, &texture);
  return Texture(texture);
}

auto genTextures(GLsizei n) -> std::vector<Texture> {
  std::vector<GLuint> ts(n);
  glGenTextures(n, ts.data());

  std::vector<Texture> res;
  std::transform(ts.begin(), ts.end(), res.begin(),
		 [](auto t) { return Texture(t); });

  return res;
}

auto genTexture_(std::string const &filepath) -> Texture {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

  if (!data)
    throw std::runtime_error(
	fmt::format("Failed to load an image from {}", filepath));

  GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
	       GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return Texture(texture);
}

} // namespace gl

