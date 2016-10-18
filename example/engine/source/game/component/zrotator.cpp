#include "game/component/zrotator.hpp"

namespace game {
namespace component {

	ZRotator::ZRotator()
		: _engine_data(nullptr)
		, _entity(-1)
	{
	}

	ZRotator::ZRotator(engine::Data* engine_data, engine::Entity entity, float speed)
		: _engine_data(engine_data)
		, _entity(entity)
		, _speed(speed)
	{
	}

	void ZRotator::Update()
	{
		const std::size_t index = _engine_data->GetIndex(_entity);
		_engine_data->rotation.z[index] += _speed;
	}
} // component.
} // game.
