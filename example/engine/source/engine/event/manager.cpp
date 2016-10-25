#include "engine/event/manager.hpp"
#include "engine/event/keyboard.hpp"

#include "engine/graphic/renderer.hpp"
#include <GLFW/glfw3.h>
#include <fst/print.h>

namespace engine {
namespace event {
	struct Manager::Content {
		GLFWwindow* window = nullptr;
	};

	void Manager::ContentDeleter::operator()(Manager::Content* p) const
	{
		delete p;
	};

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Manager* evt_manager = (Manager*)glfwGetWindowUserPointer(window);

		bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;

		if (key == GLFW_KEY_DOWN && pressed) {
			//			evt_manager->events.visit<DownKeyDown>([](DownKeyDown evt) {
			// evt.Call(); });
			evt_manager->_evt_dispatcher->push_event(0, keyboard::OnDownKeyDown);
		} else if (key == GLFW_KEY_UP && pressed) {
			//			evt_manager->events.visit<UpKeyDown>([](UpKeyDown evt) { evt.Call();
			//});
			evt_manager->_evt_dispatcher->push_event(0, keyboard::OnUpKeyDown);
		} else if (key == GLFW_KEY_LEFT && pressed) {
			//			evt_manager->events.visit<LeftKeyDown>([](LeftKeyDown evt) {
			// evt.Call(); });
			evt_manager->_evt_dispatcher->push_event(0, keyboard::OnLeftKeyDown);
		} else if (key == GLFW_KEY_RIGHT && pressed) {
			//			evt_manager->events.visit<RightKeyDown>([](RightKeyDown evt) {
			// evt.Call(); });
			evt_manager->_evt_dispatcher->push_event(0, keyboard::OnRightKeyDown);
		}
	}

	Manager::Manager(fst::evt::dispatcher<engine::Entity>* evt_dispatcher)
		: _evt_dispatcher(evt_dispatcher)
	{
		_content.reset(new Content());
	}

	void Manager::PollEvents()
	{
		glfwPollEvents();
	}

	void Manager::SetWindowHandle(void* handle)
	{
		_content->window = (GLFWwindow*)handle;
		glfwSetKeyCallback(_content->window, key_callback);
		//			glfwSetInputMode(_content->window, GLFW_STICKY_KEYS, 1);
	}
} // math.
} // engine.
