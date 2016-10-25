#include "game/component/square.hpp"

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4

namespace game {
namespace component {
	Square::Square()
		: _engine_data(nullptr)
		, _entity(-1)
		, _shader(nullptr)
		, _vbuf(nullptr)
	{
	}

	Square::Square(engine::Data* engine_data, engine::PairEntityIndex entity_info,
		engine::graphic::Shader* shader, engine::graphic::VertexBuffer* vbuf, engine::math::Vec3 pos)
		: _engine_data(engine_data)
		, _entity(entity_info.entity)
		, _shader(shader)
		, _vbuf(vbuf)
	{
		engine_data->position.set(entity_info.index, pos);
		engine_data->rotation.set(entity_info.index, engine::math::Vec3{ 0.0f, 0.0f, 0.0f });

		const float scale = 0.1;
		engine_data->scale.set(entity_info.index, engine::math::Vec3{ scale, scale, scale });

		glUseProgram(shader->GetProgram());
		_mvp_location = glGetUniformLocation(shader->GetProgram(), "mvp_matrix");
	}

	void Square::Update()
	{
		if (_entity == -1) {
			return;
		}

		const std::size_t entity_index = _engine_data->GetIndex(_entity);

		/// @todo  Move that.
		glm::mat4 mat_proj = glm::ortho(-1.0f, // Left
			1.0f, // Right.
			-1.0f, // Bottom
			1.0f, // Top.
			0.0f, // ZNear
			1.0f); // Far

		glm::mat4 mat_view = glm::mat4(1.0f);

		const GLuint shader_program = _shader->GetProgram();
		const engine::math::Vec3 position = _engine_data->position.get(entity_index);
		const engine::math::Vec3 scale = _engine_data->scale.get(entity_index);
		const engine::math::Vec3 rotation = _engine_data->rotation.get(entity_index);

		glm::mat4 mat_model = glm::translate(glm::mat4(1.0f), glm::make_vec3((const float*)&position));
		mat_model *= glm::scale(glm::mat4(1.0f), glm::make_vec3((const float*)&scale));
		// mat_model *= glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		// mat_model *= glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mat_model *= glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 mat_mvp = mat_proj * mat_view * mat_model;

		/// @todo Don't need to do that every time ???.
		glUseProgram(shader_program);
		glUniformMatrix4fv(_mvp_location, 1, GL_FALSE, glm::value_ptr(mat_mvp));

		glBindVertexArray(_vbuf->GetVAO());

		/* draw points 0-3 from the currently bound VAO with current in-use shader */
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

} // component.
} // game.
