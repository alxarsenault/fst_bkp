#pragma once

#include <OpenGL/gltypes.h>
#include "engine/def.hpp"

namespace engine {
namespace graphic {
	class Shader {
	public:
		Shader()
		{
		}

		int Init(const char* vertex_shader, const char* fragment_shader);

		GLuint GetProgram() const
		{
			return _program;
		}

	private:
		GLuint _vertex_shader = 0;
		GLuint _fragment_shader = 0;
		GLuint _program = 0;
	} PACKED_STRUCT;
}
}
