#pragma once

#include <glad/glad.h>
#include <memory>
#include <string>
#include <tuple>

namespace ogl {

/**
 * @brief Create data for a texture from a file.
 *
 * @param file
 *
 * @return If it's not a file, return nullopt. Othrewise it returns id, width,
 * hegith and its format.
 */
auto load_texture(std::string const &file)
    -> std::tuple<GLsizei, GLsizei, GLenum, GLenum, std::shared_ptr<void>>;

auto load_shader(std::string const &filepath) -> std::string;
}  // namespace ogl

