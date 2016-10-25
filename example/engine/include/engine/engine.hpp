#pragma once

#include "def.hpp"
#include "engine/entity.hpp"
#include "engine/event/manager.hpp"
#include "engine/graphic/renderer.hpp"
#include "math.hpp"

#include <fst/event.h>
#include <fst/multi_key_vector.h>
#include <sparsehash/dense_hash_map>
#include <vector>

namespace engine {
struct Data {
	struct Node {
		std::vector<Entity> parent;
		std::vector<std::vector<Entity>> children;

		inline void push_back(Entity parent = -1);
		inline void add_child(std::size_t index, Entity entity);

	} PACKED_STRUCT;

	inline Data();

	PairEntityIndex Create();
	PairEntityIndex Create(Entity parent);

	inline std::size_t GetIndex(Entity entity) const;

	google::dense_hash_map<Entity, std::size_t> _entities;
	std::vector<Entity> entity;
	Data::Node node;
	math::soa::Vec3 position;
	math::soa::Vec3 rotation;
	math::soa::Vec3 scale;

} PACKED_STRUCT;

template <typename T, typename... Ts>
struct Core {
	Core()
		: event(&evt_dispatcher)
	{
	}

	int Init(int w, int h)
	{
		if (renderer.Init(w, h)) {
			return -1;
		}

		renderer.SetEventManager(&event);
		return 0;
	}

	void NextIteration()
	{
		event.PollEvents();
		evt_dispatcher.call_events();
		renderer.SwapBuffer();
	}

	Data data;
	fst::multi_key_vector<8192, engine::Entity, T, Ts...> component;
	graphic::Renderer renderer;
	fst::evt::dispatcher<engine::Entity> evt_dispatcher;
	event::Manager event;
};
}

// Inline implementation.
namespace engine {
void Data::Node::push_back(Entity parent_)
{
	parent.emplace_back(parent_);
	children.emplace_back(std::vector<Entity>());
}

void Data::Node::add_child(std::size_t index, Entity entity)
{
	children[index].emplace_back(entity);
}

/**
 *
 */
Data::Data()
{
	_entities.set_empty_key(-1);
	_entities[0] = 0;
}

std::size_t Data::GetIndex(Entity entity) const
{
	const auto p_it = _entities.find(entity);
	return p_it != _entities.end() ? p_it->second : 0;
}
}
