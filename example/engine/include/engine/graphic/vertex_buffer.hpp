#pragma once

#include <cstddef>
#include <OpenGL/gltypes.h>
#include "engine/def.hpp"

namespace engine {
namespace graphic {
	class VertexBuffer {
	public:
		VertexBuffer()
		{
		}

		int Init(std::size_t n_elements, const float* data);

		GLuint GetVAO() const
		{
			return _vao;
		}

		GLuint GetVBO() const
		{
			return _vbo;
		}

	private:
		GLuint _vao = 0;
		GLuint _vbo = 0;
	} PACKED_STRUCT;
}
}
