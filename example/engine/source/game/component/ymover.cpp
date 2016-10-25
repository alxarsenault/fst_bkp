#include "game/component/ymover.hpp"

namespace game {
namespace component {
	YMover::YMover()
		: _engine_data(nullptr)
		, _entity(-1)
	{
	}

	YMover::YMover(engine::Data* engine_data, engine::Entity entity, float speed)
		: _engine_data(engine_data)
		, _entity(entity)
		, _speed(speed)
	{
	}

	void YMover::Update()
	{
		const std::size_t index = _engine_data->GetIndex(_entity);
		float& old_value = _engine_data->position.y[index];
		float value = old_value + (_positive_y ? _speed : -_speed);

		if (value > 0.95) {
			value = 0.95;
			_positive_y = !_positive_y;
		} else if (value < -0.95) {
			value = -0.95;
			_positive_y = !_positive_y;
		}

		old_value = value;
	}
} // component.
} // game.
