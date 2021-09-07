#include "object/gl.hpp"
#include <GLES3/gl3.h>
#include <fmt/format.h>

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

// create programs
auto createProgram() -> Program { return Program(glCreateProgram()); }

auto createShader(GLenum type) -> Shader {
  return Shader(glCreateShader(type));
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
    throw std::runtime_error(
	fmt::format("ERROR: Failed to compile shader code. glError={}, log=\"{}\" \n{}",
		    glGetError(), log, string[0]));
  }
  return Shader(shader);
}

// Gen-Functions
auto GenVertexArray() -> VertexArray {
  GLuint vao;
  glGenVertexArrays(1, &vao);
  return VertexArray(vao);
}

auto GenVertexArrays(GLsizei n) -> std::vector<VertexArray> {
  std::vector<GLuint> vaos(n);
  glGenVertexArrays(n, vaos.data());

  std::vector<VertexArray> res;
  std::transform(vaos.begin(), vaos.end(), res.begin(),
		 [](auto vao) { return VertexArray(vao); });

  return res;
}

auto GenBuffer() -> Buffer {
  GLuint bo;
  glGenBuffers(1, &bo);
  return Buffer(bo);
}

auto GenBuffers(GLsizei n) -> std::vector<Buffer> {
  std::vector<GLuint> bos;
  glGenBuffers(n, bos.data());

  std::vector<Buffer> res;
  std::transform(bos.begin(), bos.end(), res.begin(),
		 [](auto bo) { return Buffer(bo); });

  return res;
}
} // namespace gl

