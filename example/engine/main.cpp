#include <fst/multi_key_vector.h>
#include <fst/print.h>

#include <chrono>

#include "engine/engine.hpp"
#include "engine/event/keyboard.hpp"
#include "engine/graphic/renderer.hpp"
#include "engine/graphic/shader.hpp"
#include "engine/graphic/vertex_buffer.hpp"

#include "game/game.hpp"

float GetRandomValue(float left, float right)
{
	const int delta = right - left;
	return ((rand() % int(delta * 100)) + left * 100) / 100.0f;
}

class TriangleData {
public:
	int Init()
	{
		const char* vertex_shader = "#version 410\n"
									"layout(location = 0) in vec3 vertex_pos;"
									"uniform mat4 mvp_matrix;"
									"void main () {"
									"	gl_Position = mvp_matrix * vec4(vertex_pos, 1.0);"
									"}";

		/* the fragment shader colours each fragment (pixel-sized area of the
		 triangle) */
		const char* fragment_shader = "#version 410\n"
									  "out vec4 frag_colour;"
									  "void main () {"
									  "	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
									  "}";

		if (shader.Init(vertex_shader, fragment_shader)) {
			return -1;
		}

		const float points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };

		vbuf.Init(9, points);
		return 0;
	}

	engine::graphic::Shader shader;
	engine::graphic::VertexBuffer vbuf;
};

class SquareData {
public:
	int Init()
	{
		const char* vertex_shader = "#version 410\n"
									"layout(location = 0) in vec3 vertex_pos;"
									"uniform mat4 mvp_matrix;"
									"void main () {"
									"	gl_Position = mvp_matrix * vec4(vertex_pos, 1.0);"
									"}";

		/* the fragment shader colours each fragment (pixel-sized area of the
		 triangle) */
		const char* fragment_shader = "#version 410\n"
									  "out vec4 frag_colour;"
									  "void main () {"
									  "	frag_colour = vec4 (0.0, 0.0, 0.7, 1.0);"
									  "}";

		if (shader.Init(vertex_shader, fragment_shader)) {
			return -1;
		}

		const float vertices[] = {
			0.5f, 0.5f, 0.0f, // bottom left.
			-0.5f, 0.5f, 0.0f, // bottom right.
			-0.5f, -0.5f, 0.0f, // top rght.

			0.5f, 0.5f, 0.0f, // bottom left.
			-0.5f, -0.5f, 0.0f, // top rght.
			0.5f, -0.5f, 0.0f // top left.
		};

		vbuf.Init(18, vertices);
		return 0;
	}

	engine::graphic::Shader shader;
	engine::graphic::VertexBuffer vbuf;
};

struct Player {
	Player(game::EngineCore* engine_core_, SquareData* square_data)
		: engine_core(engine_core_)
	{
		const engine::PairEntityIndex entity_info = engine_core->data.Create(0);
		engine_core->component.push_back(
			entity_info.entity, game::component::Square(&engine_core->data, entity_info, &square_data->shader,
									&square_data->vbuf));

		engine_core->component.push_back(
			entity_info.entity, game::component::ZRotator(&engine_core->data, entity_info.entity, -0.3));

		entity = entity_info.entity;

		engine_core->evt_dispatcher.connect(
			0, engine::event::keyboard::OnDownKeyDown, fst::evt::function(this, &Player::OnDownKeyDown));

		engine_core->evt_dispatcher.connect(
			0, engine::event::keyboard::OnUpKeyDown, fst::evt::function(this, &Player::OnUpKeyDown));

		engine_core->evt_dispatcher.connect(
			0, engine::event::keyboard::OnLeftKeyDown, fst::evt::function(this, &Player::OnLeftKeyDown));

		engine_core->evt_dispatcher.connect(
			0, engine::event::keyboard::OnRightKeyDown, fst::evt::function(this, &Player::OnRightKeyDown));
	}

	void OnDownKeyDown()
	{
		const std::size_t index = engine_core->data.GetIndex(entity);
		engine_core->data.position.y[index] -= 0.05;
	}

	void OnUpKeyDown()
	{
		const std::size_t index = engine_core->data.GetIndex(entity);
		engine_core->data.position.y[index] += 0.05;
	}

	void OnLeftKeyDown()
	{
		const std::size_t index = engine_core->data.GetIndex(entity);
		engine_core->data.position.x[index] -= 0.05;
	}

	void OnRightKeyDown()
	{
		const std::size_t index = engine_core->data.GetIndex(entity);
		engine_core->data.position.x[index] += 0.05;
	}

	game::EngineCore* engine_core;
	engine::Entity entity;
};

int main()
{
	srand(time(nullptr));
	game::EngineCore engine_core;

	if (engine_core.Init(650, 450)) {
		return -1;
	}

	TriangleData triangle_data;
	triangle_data.Init();

	SquareData square_data;
	square_data.Init();

	Player player(&engine_core, &square_data);

	// Create triangles.
	for (int i = 0; i < 2000; i++) {
		const engine::PairEntityIndex entity_info = engine_core.data.Create(0);
		const float x = GetRandomValue(-1.0f, 1.0f) * 0.95;
		const float z = GetRandomValue(-1.0f, 1.0f) * 0.95;
		const float y = GetRandomValue(-1.0f, 1.0f) * 0.95;
		engine_core.component.push_back(entity_info.entity,
			game::component::Triangle(&engine_core.data, entity_info, &triangle_data.shader,
				&triangle_data.vbuf, engine::math::Vec3{ x, y, z }));

		const float scale = (((rand() % 100) / 100.0f) * 0.05) + 0.01;
		engine_core.data.scale.x[entity_info.index] = scale;
		engine_core.data.scale.y[entity_info.index] = scale;

		const float x_speed = ((((rand() % 200) - 100) / 100.0f) * 0.02) + 0.001;
		engine_core.component.push_back(
			entity_info.entity, game::component::XMover(&engine_core.data, entity_info.entity, x_speed));

		const float y_speed = ((((rand() % 200) - 100) / 100.0f) * 0.02) + 0.001;
		engine_core.component.push_back(
			entity_info.entity, game::component::YMover(&engine_core.data, entity_info.entity, y_speed));

		const float rot_speed = (((rand() % 200) - 100) / 100.0f) * 0.2;
		engine_core.component.push_back(
			entity_info.entity, game::component::ZRotator(&engine_core.data, entity_info.entity, rot_speed));

		engine_core.component.push_back(entity_info.entity,
			game::component::DontTouch(&engine_core.data, entity_info.entity, player.entity));
	}

	// Create squares.
	for (int i = 0; i < 2000; i++) {
		const engine::PairEntityIndex entity_info = engine_core.data.Create(0);
		const float x = GetRandomValue(-1.0f, 1.0f) * 0.95;
		const float z = GetRandomValue(-1.0f, 1.0f) * 0.95;
		const float y = GetRandomValue(-1.0f, 1.0f) * 0.95;
		engine_core.component.push_back(
			entity_info.entity, game::component::Square(&engine_core.data, entity_info, &square_data.shader,
									&square_data.vbuf, engine::math::Vec3{ x, y, z }));

		const float scale = GetRandomValue(0.0f, 1.0f) * 0.05 + 0.01;
		engine_core.data.scale.x[entity_info.index] = scale;
		engine_core.data.scale.y[entity_info.index] = scale;

		const float x_speed = GetRandomValue(-1.0f, 1.0f) * 0.02 + 0.001;
		engine_core.component.push_back(
			entity_info.entity, game::component::XMover(&engine_core.data, entity_info.entity, x_speed));

		const float y_speed = GetRandomValue(-1.0f, 1.0f) * 0.02 + 0.001;
		engine_core.component.push_back(
			entity_info.entity, game::component::YMover(&engine_core.data, entity_info.entity, y_speed));

		const float rot_speed = GetRandomValue(-1.0f, 1.0f) * 0.2;
		engine_core.component.push_back(
			entity_info.entity, game::component::ZRotator(&engine_core.data, entity_info.entity, rot_speed));

		engine_core.component.push_back(entity_info.entity,
			game::component::DontTouch(&engine_core.data, entity_info.entity, player.entity));
	}

	// Tell GL to only draw onto a pixel if the shape is closer to the viewer than
	// anything already drawn at
	// that pixel.
	glEnable(GL_DEPTH_TEST);

	// With LESS depth-testing interprets a smaller depth value as meaning
	// "closer".
	glDepthFunc(GL_LESS);

	auto start_time = std::chrono::high_resolution_clock::now();
	int fps_count = 0;
	double ms_sum = 0;
	const int fsp_max_count = 60;

	while (engine_core.renderer.IsActive()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		engine_core.component.visit([](auto& c) { c.Update(); });
		engine_core.NextIteration();

		auto end = std::chrono::high_resolution_clock::now();
		ms_sum += 1000.0 / (std::chrono::duration<double>(end - start_time).count() * 1000.0);
		start_time = end;

		if (fps_count == fsp_max_count) {
			fst::print("FPS :", ms_sum / double(fsp_max_count));
			fps_count = 0;
			ms_sum = 0.0;
		}

		fps_count++;
	}

	return 0;
}
