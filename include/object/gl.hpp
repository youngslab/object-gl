#pragma once

#include <raii/raii.hpp>
#include <GLES3/gl3.h>

namespace gl {

class Shader : public raii::AutoDeletable<GLuint> {
public:
  Shader(GLuint);
};

class Program : public raii::AutoDeletable<GLuint> {
public:
  Program(GLuint);
};

auto createProgram() -> Program;
auto createProgram_(GLuint vertexShader, GLuint fragmentShader) -> Program;

auto createShader(GLenum type) -> Shader;
auto createShader_(GLenum type, GLsizei count, const GLchar *const *string,
		   const GLint *length) -> Shader;

} // namespace gl

