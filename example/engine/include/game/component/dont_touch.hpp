#pragma once

#include "engine/engine.hpp"

namespace game {
namespace component {
	class DontTouch {
	public:
		DontTouch();
		DontTouch(engine::Data* engine_data, engine::Entity entity, engine::Entity dont_touch);

		void Update();

	private:
		engine::Data* _engine_data;
		engine::Entity _entity;
		engine::Entity _dont_touch;
	};
} // component.
} // game.
