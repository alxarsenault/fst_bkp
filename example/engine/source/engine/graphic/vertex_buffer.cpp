#include "engine/graphic/vertex_buffer.hpp"
#include <OpenGL/gl3.h>

namespace engine {
namespace graphic {
	int VertexBuffer::Init(std::size_t n_elements, const float* data)
	{
		/* a vertex buffer object (VBO) is created here. this stores an array of
		 data on the graphics adapter's memory. in our case - the vertex points */
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, n_elements * sizeof(float), data, GL_STATIC_DRAW);

		/* the vertex array object (VAO) is a little descriptor that defines which
		 data from vertex buffer objects should be used as input variables to vertex
		 shaders. in our case - use our only VBO, and say 'every three floats is a
		 variable' */
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		glEnableVertexAttribArray(0); // "attribute #0 should be enabled when this vao is bound"

		// this VBO is already bound, but it's a good habit to explicitly specify which VBO's data the
		// following
		// vertex attribute pointer refers to
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// "attribute #0 is created from every 3 variables in the above buffer, of type float (i.e. make me
		// vec3s)"
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		return 0;
	}
}
}
