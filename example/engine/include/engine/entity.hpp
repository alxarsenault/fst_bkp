#pragma once

#include <cstddef>

namespace engine {
typedef long Entity;

struct PairEntityIndex {
	Entity entity;
	std::size_t index;
};
} // engine.
