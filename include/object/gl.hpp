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

class VertexArray : public raii::AutoDeletable<GLuint> {
public:
  VertexArray(GLuint);
};

class Buffer : public raii::AutoDeletable<GLuint> {
public:
  Buffer(GLuint);
};

class Texture : public raii::AutoDeletable<GLuint> {
public:
  Texture(GLuint);
};

auto createProgram() -> Program;
auto createProgram_(GLuint vertexShader, GLuint fragmentShader) -> Program;
auto createProgram_(std::string const &vertex, std::string const &fragment)
    -> Program;

auto createShader(GLenum type) -> Shader;
auto createShader_(GLenum type, GLsizei count, const GLchar *const *string,
		   const GLint *length) -> Shader;

auto genVertexArray() -> VertexArray;
auto genVertexArrays(GLsizei n) -> std::vector<VertexArray>;

auto genBuffer() -> Buffer;
auto genBuffers(GLsizei n) -> std::vector<Buffer>;

auto genTexture() -> Texture;
auto genTexture_(std::string const &filepath) -> Texture;
auto genTextures(GLsizei n) -> std::vector<Texture>;

} // namespace gl

