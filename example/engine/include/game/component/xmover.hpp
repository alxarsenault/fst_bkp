#include "engine/engine.hpp"

namespace game {
namespace component {
	class XMover {
	public:
		XMover();
		XMover(engine::Data* engine_data, engine::Entity entity, float speed = 0.0f);

		void Update();

	private:
		engine::Data* _engine_data;
		engine::Entity _entity;
		bool _positive_x = false;
		float _speed = 0.0;
	};
} // component.
} // game.
