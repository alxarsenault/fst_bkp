#include <fst/multi_key_vector.h>
#include <fst/print.h>
#include <fst/soa_struct.h>
#include <fst/unordered_key_vector.h>
#include <sparsehash/dense_hash_map>

#include <GLFW/glfw3.h> /* GLFW helper library */
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4

#include <chrono>
#include <stdio.h>

namespace engine {
typedef long Entity;

namespace scene {
	namespace type {
		enum Name {
			EntityId,
			Parent,
			Children,

			POS_X,
			POS_Y,
			POS_Z,

			ROT_X,
			ROT_Y,
			ROT_Z,

			SCALE_X,
			SCALE_Y,
			SCALE_Z
		};
	}

	typedef fst::soa_type<type::EntityId, long> EntityId;
	typedef fst::soa_type<type::Parent, long> Parent;
	typedef fst::soa_type<type::Children, std::vector<long>> Children;

	typedef fst::soa_type<type::POS_X, float> POS_X;
	typedef fst::soa_type<type::POS_Y, float> POS_Y;
	typedef fst::soa_type<type::POS_Z, float> POS_Z;

	typedef fst::soa_type<type::ROT_X, float> ROT_X;
	typedef fst::soa_type<type::ROT_Y, float> ROT_Y;
	typedef fst::soa_type<type::ROT_Z, float> ROT_Z;

	typedef fst::soa_type<type::SCALE_X, float> SCALE_X;
	typedef fst::soa_type<type::SCALE_Y, float> SCALE_Y;
	typedef fst::soa_type<type::SCALE_Z, float> SCALE_Z;

	typedef fst::soa_struct<EntityId, Parent, Children, POS_X, POS_Y, POS_Z, ROT_X, ROT_Y, ROT_Z, SCALE_X,
		SCALE_Y, SCALE_Z>
		Data;

	class Graph {
	public:
		Graph()
		{
			_entities.set_empty_key(-1);
			_entities[0] = 0;
			_data.push_back(
				0, Parent(-1), std::vector<long>(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		}

		Entity create()
		{
			static long global_entity_count = 1;
			_entities[global_entity_count] = _data.get_multi_vector().Get<POS_X>().size();
			_data.push_back(global_entity_count, Parent(-1), std::vector<long>(), 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			return global_entity_count++;
		}

		Data& get_soa_struct()
		{
			return _data;
		}

		const Data& get_soa_struct() const
		{
			return _data;
		}

		std::size_t get_entity_index(Entity entity) const
		{
			auto p_it = _entities.find(entity);
			if (p_it != _entities.end()) {
				return p_it->second;
			}

			return 0;
		}

		void insert(long parent, long child)
		{
			if (parent == 0) {
				auto c_it = _entities.find(child);
				if (c_it == _entities.end()) {
					return;
				}

				_data.get<Children>()[0].value.push_back(child);
				_data.get<Parent>()[child].value = 0;
				return;
			}

			auto p_it = _entities.find(parent);
			if (p_it == _entities.end()) {
				return;
			}

			auto c_it = _entities.find(child);
			if (c_it == _entities.end()) {
				return;
			}

			_data.get<Children>()[p_it->second].value.push_back(child);
			_data.get<Parent>()[c_it->second].value = parent;
		}

		void reparent(long parent, long child)
		{
			auto p_it = _entities.find(parent);
			if (p_it == _entities.end()) {
				return;
			}

			auto c_it = _entities.find(child);
			if (c_it == _entities.end()) {
				return;
			}

			_data.get<Children>()[p_it->second].value.push_back(child);

			// Old parent.
			long& old_parent = _data.get<Parent>()[c_it->second].value;
			auto& c_children_vec = _data.get<Children>()[c_it->second].value;

			int index = -1;

			for (int i = 0; i < (int)c_children_vec.size(); i++) {
				if (c_children_vec[i] == child) {
					index = i;
					break;
				}
			}

			if (index != -1) {
				c_children_vec.erase(c_children_vec.begin(), c_children_vec.begin() + index);
			}

			old_parent = parent;
		}

		void remove()
		{
		}

	private:
		google::dense_hash_map<Entity, std::size_t> _entities;
		Data _data;
	};
}
}

namespace global {
engine::scene::Graph graph;
}

namespace component {
class Triangle {
public:
	Triangle()
		: _entity(-1)
	{
	}

	Triangle(engine::Entity entity, GLuint shader_id, GLuint vao)
		: _entity(entity)
		, _shader_id(shader_id)
		, _vao(vao)
	{
	}

	void Update()
	{
		if (_entity == -1) {
			return;
		}

		const engine::scene::Data& data = global::graph.get_soa_struct();
		const std::size_t index = global::graph.get_entity_index(_entity);
		const float pos_x = ((float*)data.get<engine::scene::POS_X>())[index];
		const float pos_y = ((float*)data.get<engine::scene::POS_Y>())[index];
		const float pos_z = ((float*)data.get<engine::scene::POS_Z>())[index];

		const float rot_x = ((float*)data.get<engine::scene::ROT_X>())[index];
		const float rot_y = ((float*)data.get<engine::scene::ROT_Y>())[index];
		const float rot_z = ((float*)data.get<engine::scene::ROT_Z>())[index];

		const float scale_x = ((float*)data.get<engine::scene::SCALE_X>())[index];
		const float scale_y = ((float*)data.get<engine::scene::SCALE_Y>())[index];
		const float scale_z = ((float*)data.get<engine::scene::SCALE_Z>())[index];

		/// @todo  Move that.
		glm::mat4 mat_proj = glm::ortho(-1.0f, // Left
			1.0f, // Right.
			-1.0f, // Bottom
			1.0f, // Top.
			0.0f, // ZNear
			1.0f); // Far

		glm::mat4 mat_view = glm::mat4(1.0f);

		glm::mat4 mat_model = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, pos_z));
		mat_model *= glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, scale_y, scale_z));
		mat_model *= glm::rotate(rot_x, glm::vec3(1.0f, 0.0f, 0.0f));
		mat_model *= glm::rotate(rot_y, glm::vec3(0.0f, 1.0f, 0.0f));
		mat_model *= glm::rotate(rot_z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 mat_mvp = mat_proj * mat_view * mat_model;

		/// @todo Don't need to do that every time.
		GLint mvp_location = glGetUniformLocation(_shader_id, "mvp_matrix");
		glUseProgram(_shader_id);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mat_mvp));

		glBindVertexArray(_vao);

		/* draw points 0-3 from the currently bound VAO with current in-use shader
		 */
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	engine::Entity _entity;
	GLuint _shader_id;
	GLuint _vao;
};

class Rotator {
public:
	Rotator()
		: _entity(-1)
	{
	}

	Rotator(engine::Entity entity)
		: _entity(entity)
	{
	}

	void Update()
	{
		engine::scene::Data& data = global::graph.get_soa_struct();
		const std::size_t index = global::graph.get_entity_index(_entity);
		((float*)data.get<engine::scene::ROT_Z>())[index] += 0.01;
	}

private:
	engine::Entity _entity;
};

class FastRotator {
public:
	FastRotator()
		: _entity(-1)
	{
	}

	FastRotator(engine::Entity entity)
		: _entity(entity)
	{
	}

	void Update()
	{
		engine::scene::Data& data = global::graph.get_soa_struct();
		const std::size_t index = global::graph.get_entity_index(_entity);
		((float*)data.get<engine::scene::ROT_Z>())[index] += 0.1;
	}

private:
	engine::Entity _entity;
};

class RandomMover {
public:
	RandomMover()
		: _entity(-1)
	{
	}

	RandomMover(engine::Entity entity)
		: _entity(entity)
	{
	}

	void Update()
	{
		engine::scene::Data& data = global::graph.get_soa_struct();
		const std::size_t index = global::graph.get_entity_index(_entity);
		float& old_value = ((float*)data.get<engine::scene::POS_X>())[index];
		float value = old_value + (_positive_x ? 0.008 : -0.008);

		if (_positive_x) {
			if (value > 0.95) {
				value -= 0.008;
				_positive_x = false;
			}
		} else {
			if (value < -0.95) {
				value += 0.008;
				_positive_x = true;
			}
		}

		old_value = value;
	}

private:
	engine::Entity _entity;
	bool _positive_x = false;
};
}

namespace global {
fst::multi_key_vector<1024, engine::Entity, component::Triangle, component::Rotator, component::FastRotator,
	component::RandomMover>
	components;
}

void AddTriangle(GLuint shader_program, GLuint vao)
{
	engine::Entity entity = global::graph.create();
	global::components.push_back(entity, component::Triangle(entity, shader_program, vao));
	global::graph.insert(0, entity);
	auto& data = global::graph.get_soa_struct();
	const std::size_t data_index = global::graph.get_entity_index(entity);
	data.get<engine::scene::POS_X>()[data_index] = ((rand() % 200) - 100.0) / 100.0;
	data.get<engine::scene::POS_Y>()[data_index] = ((rand() % 200) - 100.0) / 100.0;
	data.get<engine::scene::POS_Z>()[data_index] = 0.0f;

	data.get<engine::scene::ROT_X>()[data_index] = 0.0f;
	data.get<engine::scene::ROT_Y>()[data_index] = 0.0f;
	data.get<engine::scene::ROT_Z>()[data_index] = (((rand() % 200) - 100.0) / 100.0) * 360.0;

	const float scale = ((rand() % 100) / 100.0) * 0.03 + 0.05;
	data.get<engine::scene::SCALE_X>()[data_index] = scale;
	data.get<engine::scene::SCALE_Y>()[data_index] = scale;
	data.get<engine::scene::SCALE_Z>()[data_index] = scale;

	// Add RandomMover to triangle.
	global::components.push_back(entity, component::RandomMover(entity));

	// Add rotator to triangle.
	global::components.push_back(entity, component::FastRotator(entity));
}

int main()
{
	srand(time(NULL));
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;
	GLuint vao;
	GLuint vbo;

	/* geometry to use. these are 3 xyz points (9 floats total) to make a triangle
	 */
	GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
	/* these are the strings of code for the shaders
	 the vertex shader positions each vertex point */
	const char* vertex_shader = "#version 410\n"
								"layout(location = 0) in vec3 vertex_pos;"
								"uniform mat4 mvp_matrix;"
								"void main () {"
								"	gl_Position = mvp_matrix * vec4(vertex_pos, 1.0);"
								//	"	gl_Position = vec4(vertex_pos, 1.0);"

								"}";

	/* the fragment shader colours each fragment (pixel-sized area of the
	 triangle) */
	const char* fragment_shader = "#version 410\n"
								  "out vec4 frag_colour;"
								  "void main () {"
								  "	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
								  "}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vert_shader, frag_shader;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme;

	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	/* We must specify 3.2 core if on Apple OS X -- other O/S can specify
	 anything here. I defined 'APPLE' in the makefile for OS X */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	/* get version info */
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION); /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer
	 than anything already drawn at that pixel */
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */

	/* with LESS depth-testing interprets a smaller depth value as meaning
	 * "closer" */
	glDepthFunc(GL_LESS);

	/* a vertex buffer object (VBO) is created here. this stores an array of
	 data on the graphics adapter's memory. in our case - the vertex points */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	/* the vertex array object (VAO) is a little descriptor that defines which
	 data from vertex buffer objects should be used as input variables to vertex
	 shaders. in our case - use our only VBO, and say 'every three floats is a
	 variable' */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0); // "attribute #0 should be enabled when this vao is bound"

	// this VBO is already bound, but it's a good habit to explicitly specify
	// which VBO's data the following
	// vertex attribute pointer refers to
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// "attribute #0 is created from every 3 variables in the above buffer, of
	// type float (i.e. make me
	// vec3s)"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	/* here we copy the shader strings into GL shaders, and compile them. we
	 then create an executable shader 'program' and attach both of the compiled
	 shaders. we link this, which matches the outputs of the vertex shader to
	 the inputs of the fragment shader, etc. and it is then ready to use */
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);

	GLint isCompiled = 0;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		int info_length = 0;
		glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_length);

		if (info_length > 1) {
			int n_char_written = 0;
			char* error_msg = new char[info_length];
			glGetShaderInfoLog(vert_shader, info_length, &n_char_written, error_msg);

			fst::print("Vertex Shader :", error_msg);
			// ax::console::Error("Vertex Shader :", error_msg);
			delete[] error_msg;
		}
	} else {
		fst::print("All good");
	}

	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, frag_shader);
	glAttachShader(shader_programme, vert_shader);
	glLinkProgram(shader_programme);

	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------

	// Add triangle.
	{
		engine::Entity entity = global::graph.create();
		global::components.push_back(entity, component::Triangle(entity, shader_programme, vao));
		global::graph.insert(0, entity);
		auto& data = global::graph.get_soa_struct();
		const std::size_t data_index = global::graph.get_entity_index(entity);
		data.get<engine::scene::POS_X>()[data_index] = -0.8f;
		data.get<engine::scene::POS_Y>()[data_index] = 0.0f;
		data.get<engine::scene::POS_Z>()[data_index] = 0.0f;

		data.get<engine::scene::ROT_X>()[data_index] = 0.0f;
		data.get<engine::scene::ROT_Y>()[data_index] = 0.0f;
		data.get<engine::scene::ROT_Z>()[data_index] = 0.0f;

		data.get<engine::scene::SCALE_X>()[data_index] = 0.3f;
		data.get<engine::scene::SCALE_Y>()[data_index] = 0.3f;
		data.get<engine::scene::SCALE_Z>()[data_index] = 0.3f;

		// Add RandomMover to triangle.
		global::components.push_back(entity, component::RandomMover(entity));

		// Add rotator to triangle.
		global::components.push_back(entity, component::Rotator(entity));
	}

	// Add triangle.
	{
		engine::Entity entity = global::graph.create();
		global::components.push_back(entity, component::Triangle(entity, shader_programme, vao));
		global::graph.insert(0, entity);
		auto& data = global::graph.get_soa_struct();
		const std::size_t data_index = global::graph.get_entity_index(entity);
		data.get<engine::scene::POS_X>()[data_index] = 0.6f;
		data.get<engine::scene::POS_Y>()[data_index] = 0.4f;
		data.get<engine::scene::POS_Z>()[data_index] = 0.0f;

		data.get<engine::scene::ROT_X>()[data_index] = 0.0f;
		data.get<engine::scene::ROT_Y>()[data_index] = 0.0f;
		data.get<engine::scene::ROT_Z>()[data_index] = 0.0f;

		data.get<engine::scene::SCALE_X>()[data_index] = 0.2f;
		data.get<engine::scene::SCALE_Y>()[data_index] = 0.2f;
		data.get<engine::scene::SCALE_Z>()[data_index] = 0.2f;

		// Add rotator to triangle.
		global::components.push_back(entity, component::Rotator(entity));
	}

	// Add triangle.
	{
		engine::Entity entity = global::graph.create();
		global::components.push_back(entity, component::Triangle(entity, shader_programme, vao));
		global::graph.insert(0, entity);
		auto& data = global::graph.get_soa_struct();
		const std::size_t data_index = global::graph.get_entity_index(entity);
		data.get<engine::scene::POS_X>()[data_index] = 0.0f;
		data.get<engine::scene::POS_Y>()[data_index] = 0.5f;
		data.get<engine::scene::POS_Z>()[data_index] = 0.0f;

		data.get<engine::scene::ROT_X>()[data_index] = 0.0f;
		data.get<engine::scene::ROT_Y>()[data_index] = 0.0f;
		data.get<engine::scene::ROT_Z>()[data_index] = 0.0f;

		data.get<engine::scene::SCALE_X>()[data_index] = 0.1f;
		data.get<engine::scene::SCALE_Y>()[data_index] = 0.1f;
		data.get<engine::scene::SCALE_Z>()[data_index] = 0.1f;

		// Add rotator to triangle.
		global::components.push_back(entity, component::FastRotator(entity));
	}

	for (int i = 0; i < 1000; i++) {
		AddTriangle(shader_programme, vao);
	}

	auto start_time = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window)) {
		// Wipe the drawing surface clear.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		global::components.visit([](auto& c) { c.Update(); });

		// Update other events like input handling.
		glfwPollEvents();

		// Put the stuff we've been drawing onto the display.
		glfwSwapBuffers(window);

		auto end = std::chrono::high_resolution_clock::now();

		fst::print("FPS :", 1000.0 / (std::chrono::duration<double>(end - start_time).count() * 1000.0));
		start_time = end;
	}

	/* close GL context and any other GLFW resources */
	glfwTerminate();
	return 0;
}
