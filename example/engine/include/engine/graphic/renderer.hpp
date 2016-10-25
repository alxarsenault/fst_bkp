#pragma once

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <memory>

namespace engine {
namespace event {
	class Manager;
}
namespace graphic {
	class Renderer {
	public:
		Renderer();
		~Renderer() = default;

		int Init(int w, int h);

		void SwapBuffer();

		bool IsActive() const;

		void SetEventManager(event::Manager* evt_manager);

	private:
		struct Content;

		struct ContentDeleter { // deleter
			ContentDeleter(){};
			ContentDeleter(const ContentDeleter&)
			{
			}
			ContentDeleter(ContentDeleter&)
			{
			}
			ContentDeleter(ContentDeleter&&)
			{
			}
			void operator()(Content* p) const;
		};

		std::unique_ptr<Content, ContentDeleter> _content;
	};
} // graphic.
} // engine.
