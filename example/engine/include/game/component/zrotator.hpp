#include "engine/engine.hpp"

namespace game {
namespace component {
	class ZRotator {
	public:
		ZRotator();
		ZRotator(engine::Data* engine_data, engine::Entity entity, float speed = 0.0);

		void Update();

	private:
		engine::Data* _engine_data;
		engine::Entity _entity;
		float _speed = 0.0;
	};
} // component.
} // game.
