#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Definitions.h>

class Shader
{
public:
    // program id
    uint32_t m_Id;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    // use/activate the shader
    void Bind() const;
    void Unbind() const;
    // utility uniform functions
    template<typename T>
    void Set(const std::string& name, T value) const;
    int GetUniformLocation(const std::string& name) const;

};

// this needs to be here
template<typename T>
inline void Shader::Set(const std::string& name, T value) const
{
    // absolute garbage code, someone please stop me
    static_assert(
        std::is_same_v<T, glm::f32vec1> ||
        std::is_same_v<T, glm::f32vec2> ||
        std::is_same_v<T, glm::f32vec2> ||
        std::is_same_v<T, glm::f32vec3> ||
        std::is_same_v<T, glm::f32vec4> ||
        std::is_same_v<T, glm::u32vec1> ||
        std::is_same_v<T, glm::u32vec2> ||
        std::is_same_v<T, glm::u32vec3> ||
        std::is_same_v<T, glm::u32vec4> ||
        std::is_same_v<T, glm::i32vec1> ||
        std::is_same_v<T, glm::i32vec2> ||
        std::is_same_v<T, glm::i32vec3> ||
        std::is_same_v<T, glm::i32vec4> ||
        std::is_same_v<T, glm::mat3x3> ||
        std::is_same_v<T, glm::mat4x4>

        );

    const int uniformLoc = glGetUniformLocation(m_Id, name.c_str());

    if constexpr (std::is_same_v<T, glm::f32vec1>)
    {
        glUniform1f(uniformLoc, value.x);
    }
    else if constexpr (std::is_same_v<T, glm::f32vec2>)
    {
        glUniform2f(uniformLoc, value.x, value.y);
    }
    else if constexpr (std::is_same_v<T, glm::f32vec3>)
    {
        glUniform3f(uniformLoc, value.x, value.y, value.z);
    }
    else if constexpr (std::is_same_v<T, glm::f32vec4>)
    {
        glUniform4f(uniformLoc, value.x, value.y, value.z, value.w);
    }
    else if constexpr (std::is_same_v<T, glm::u32vec1>)
    {
        glUniform1ui(uniformLoc, value.x);
    }
    else if constexpr (std::is_same_v<T, glm::u32vec2>)
    {
        glUniform2ui(uniformLoc, value.x, value.y);
    }
    else if constexpr (std::is_same_v<T, glm::u32vec3>)
    {
        glUniform3ui(uniformLoc, value.x, value.y, value.z);
    }
    else if constexpr (std::is_same_v<T, glm::u32vec4>)
    {
        glUniform4ui(uniformLoc, value.x, value.y, value.z, value.w);
    }
    else if constexpr (std::is_same_v<T, glm::i32vec1>)
    {
        glUniform1i(uniformLoc, value.x);
    }
    else if constexpr (std::is_same_v<T, glm::i32vec2>)
    {
        glUniform2i(uniformLoc, value.x, value.y);
    }
    else if constexpr (std::is_same_v<T, glm::i32vec3>)
    {
        glUniform3i(uniformLoc, value.x, value.y, value.z);
    }
    else if constexpr (std::is_same_v<T, glm::i32vec4>)
    {
        glUniform4i(uniformLoc, value.x, value.y, value.z, value.w);
    }
    else if constexpr (std::is_same_v<T, glm::mat3x3>)
    {
        glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<T, glm::mat4x4>)
    {
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
    }
}

