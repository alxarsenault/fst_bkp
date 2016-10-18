#include "engine/graphic/shader.hpp"
#include <fst/print.h>
#include <OpenGL/gl.h>

namespace engine {
namespace graphic {
	bool Compile(GLuint shader)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_TRUE) {
			return true;
		}

		int info_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);

		if (info_length > 1) {
			int n_char_written = 0;
			char* error_msg = new char[info_length];
			glGetShaderInfoLog(shader, info_length, &n_char_written, error_msg);
			fst::errprint(error_msg);
			delete[] error_msg;
		}

		return false;
	}

	bool Link(GLuint program, GLuint vertex_shader, GLuint fragment_shader)
	{
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		GLint islinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &islinked);

		if (islinked == GL_TRUE) {
			return true;
		}

		int info_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);

		if (info_length > 1) {
			int n_char_written = 0;
			char* error_msg = new char[info_length];
			glGetProgramInfoLog(program, info_length, &n_char_written, error_msg);
			fst::errprint(error_msg);
			delete[] error_msg;
		}

		return false;
	}

	int Shader::Init(const char* vertex_shader, const char* fragment_shader)
	{
		// Compile vertex shader.
		_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(_vertex_shader, 1, &vertex_shader, nullptr);
		glCompileShader(_vertex_shader);

		if (!Compile(_vertex_shader)) {
			fst::errprint("Compile vert");
			return 1;
		}

		_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(_fragment_shader, 1, &fragment_shader, nullptr);
		glCompileShader(_fragment_shader);

		if (!Compile(_fragment_shader)) {
			fst::errprint("Compile frag");
			return 1;
		}

		// Create shader program.
		_program = glCreateProgram();
		if (!Link(_program, _vertex_shader, _fragment_shader)) {
			fst::errprint("Link");
			return 2;
		}

		return 0;
	}
}
}
