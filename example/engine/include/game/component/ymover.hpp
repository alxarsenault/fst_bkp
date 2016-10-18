#include "engine/engine.hpp"

namespace game {
namespace component {
	class YMover {
	public:
		YMover();
		YMover(engine::Data* engine_data, engine::Entity entity, float speed = 0.0);

		void Update();

	private:
		engine::Data* _engine_data;
		engine::Entity _entity;
		bool _positive_y = false;
		float _speed = 0.0;
	};
} // component.
} // game.
