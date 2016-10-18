#pragma once

#include <vector>
#include <fst/multi_vector.h>

namespace engine {
namespace event {
	struct DownKeyDown {
		void (*func)(void* data);
		void* data;

		void Call()
		{
			func(data);
		}
	};

	struct UpKeyDown {
		void (*func)(void* data);
		void* data;

		void Call()
		{
			func(data);
		}
	};

	struct LeftKeyDown {
		void (*func)(void* data);
		void* data;

		void Call()
		{
			func(data);
		}
	};

	struct RightKeyDown {
		void (*func)(void* data);
		void* data;

		void Call()
		{
			func(data);
		}
	};

	class Manager {
	public:
		Manager();
		~Manager() = default;

		void SetWindowHandle(void* handle);

		template <typename T> void Connect(const T& evt)
		{
			events.push_back(evt);
		}

		fst::multi_vector<DownKeyDown, UpKeyDown, RightKeyDown, LeftKeyDown> events;

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
}
}
