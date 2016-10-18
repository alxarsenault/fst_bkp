#include "engine/engine.hpp"

namespace engine {
namespace global {
	Entity entity_count = 1;
}

inline PairEntityIndex Data::Create()
{
	const std::size_t index = position.x.size();
	const Entity current_entity = global::entity_count++;

	// Add entity to map.
	_entities[current_entity] = index;

	entity.push_back(current_entity);
	node.push_back();
	position.push_back();
	rotation.push_back();
	scale.push_back();

	return PairEntityIndex{ current_entity, index };
}

PairEntityIndex Data::Create(Entity parent)
{
	if (parent < 0) {
		return Create();
	}

	const std::size_t index = position.x.size();
	const Entity current_entity = global::entity_count++;

	// Add entity to map.
	_entities[current_entity] = index;
	entity.push_back(global::entity_count);

	// Add parent.
	node.push_back(parent);

	// Add current_node to children of parent.
	const std::size_t parent_index = GetIndex(parent);
	node.add_child(parent_index, current_entity);

	position.push_back();
	rotation.push_back();
	scale.push_back();

	return PairEntityIndex{ current_entity, index };
}
}
