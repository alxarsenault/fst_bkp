#include "game/component/xmover.hpp"

namespace game {
namespace component {
	XMover::XMover()
		: _engine_data(nullptr)
		, _entity(-1)
	{
	}

	XMover::XMover(engine::Data* engine_data, engine::Entity entity, float speed)
		: _engine_data(engine_data)
		, _entity(entity)
		, _speed(speed)
	{
	}

	void XMover::Update()
	{
		const std::size_t index = _engine_data->GetIndex(_entity);
		float& old_value = _engine_data->position.x[index];
		float value = old_value + (_positive_x ? _speed : -_speed);

		if (value > 0.95) {
			value = 0.95;
			_positive_x = !_positive_x;
		} else if (value < -0.95) {
			value = -0.95;
			_positive_x = !_positive_x;
		}

		old_value = value;
	}
} // component.
} // game.
