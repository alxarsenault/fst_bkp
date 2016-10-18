#include "game/component/dont_touch.hpp"
#include <math.h>

namespace game {
namespace component {
	DontTouch::DontTouch()
		: _engine_data(nullptr)
		, _entity(-1)
	{
	}

	DontTouch::DontTouch(engine::Data* engine_data, engine::Entity entity, engine::Entity dont_touch)
		: _engine_data(engine_data)
		, _entity(entity)
		, _dont_touch(dont_touch)
	{
	}

	void DontTouch::Update()
	{
		const std::size_t index = _engine_data->GetIndex(_entity);
		const std::size_t dt_index = _engine_data->GetIndex(_dont_touch);
		const engine::math::Vec3 vec = _engine_data->position.get(dt_index);

		float& x = _engine_data->position.x[index];
		float& y = _engine_data->position.y[index];

		const float inside_delta = 0.2;
		const float away_delta = 0.3;

		if (sqrt(pow(x - vec.x, 2) + pow(y - vec.y, 2)) < inside_delta) {
			y = (rand() % 1) ? y - away_delta : y + away_delta;
			x = (rand() % 1) ? x - away_delta : x + away_delta;
		}
	}
} // component.
} // game.
