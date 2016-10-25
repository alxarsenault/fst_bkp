#pragma once
#include "engine/engine.hpp"
#include "game/component/dont_touch.hpp"
#include "game/component/square.hpp"
#include "game/component/triangle.hpp"
#include "game/component/xmover.hpp"
#include "game/component/ymover.hpp"
#include "game/component/zrotator.hpp"

namespace game {
using EngineCore = engine::Core<game::component::XMover, game::component::YMover, game::component::ZRotator,
	game::component::DontTouch, game::component::Triangle, game::component::Square>;
}
