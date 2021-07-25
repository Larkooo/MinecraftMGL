#pragma once

#include <cstdint>
#include <glm/glm.hpp>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;
	/*glm::vec2 tile;
	glm::vec2 factor;*/
};