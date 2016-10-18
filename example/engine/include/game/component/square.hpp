#pragma once

#include "engine/engine.hpp"
#include "engine/graphic/shader.hpp"
#include "engine/graphic/vertex_buffer.hpp"

namespace game {
namespace component {
	class Square {
	public:
		Square();

		Square(engine::Data* engine_data, engine::PairEntityIndex entity_info,
			engine::graphic::Shader* shader, engine::graphic::VertexBuffer* vbuf,
			engine::math::Vec3 pos = { 0.0f, 0.0f, 0.0f });

		void Update();

	private:
		engine::Data* _engine_data;
		engine::Entity _entity;
		engine::graphic::Shader* _shader;
		engine::graphic::VertexBuffer* _vbuf;
		GLuint _mvp_location = 0;
	};
} // component.
} // game.
