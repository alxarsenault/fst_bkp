#pragma once

#include <fst/event.h>

namespace engine {
namespace event {
	namespace keyboard {
		enum : fst::evt::Id { OnDownKeyDown, OnUpKeyDown, OnLeftKeyDown, OnRightKeyDown };
	}
} // event.
} // engine.
