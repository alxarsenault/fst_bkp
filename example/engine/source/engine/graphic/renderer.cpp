#include "engine/graphic/renderer.hpp"

#include "engine/event/manager.hpp"
#include <GLFW/glfw3.h>
#include <fst/print.h>

namespace engine {
namespace graphic {
	struct Renderer::Content {
		~Content()
		{
			glfwTerminate();
		}

		GLFWwindow* window = nullptr;
	};

	void Renderer::ContentDeleter::operator()(Renderer::Content* p) const
	{
		delete p;
	};

	Renderer::Renderer()
	{
		_content.reset(new Content());
	}

	int Renderer::Init(int w, int h)
	{
		// Start GL context and O/S window using the GLFW helper library.
		if (!glfwInit()) {
			fst::errprint("Could not start GLFW3.");
			return 1;
		}

		// Specify core.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		_content->window = glfwCreateWindow(w, h, "Engine", nullptr, nullptr);

		if (_content->window == nullptr) {
			fst::errprint("Could not open window with GLFW3.");
			glfwTerminate();
			return 2;
		}

		glfwMakeContextCurrent(_content->window);

		return 0;
	}

	void Renderer::SetEventManager(event::Manager* evt_manager)
	{
		evt_manager->SetWindowHandle(_content->window);
		glfwSetWindowUserPointer(_content->window, evt_manager);
	}

	void Renderer::SwapBuffer()
	{
		//		glfwPollEvents();
		glfwSwapBuffers(_content->window);
	}

	bool Renderer::IsActive() const
	{
		return !glfwWindowShouldClose(_content->window) && _content->window;
	}
} // graphic.
} // engine.
