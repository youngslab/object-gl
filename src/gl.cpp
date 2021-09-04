#include "object/gl.hpp"
#include <fmt/format.h>

namespace gl {

Shader::Shader(GLuint shader)
    : raii::AutoDeletable<GLuint>(shader,
				  [](auto shader) { glDeleteShader(shader); }) {
}

Program::Program(GLuint program)
    : raii::AutoDeletable<GLuint>(program, [](auto p) { glDeleteProgram(p); }) {
}

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
    throw std::runtime_error(fmt::format("Failed to link program. program={}, "
					 "vertexShader={}, fragmentShader={}",
					 program, vertexShader,
					 fragmentShader));
  }
  return program;
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
	fmt::format("ERROR: Failed to compile shader code. glError={} \n{}",
		    glGetError(), string[0]));
  }
  return Shader(shader);
}

} // namespace gl

