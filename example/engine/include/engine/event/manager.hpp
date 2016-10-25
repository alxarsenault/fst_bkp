#pragma once

#include "engine/entity.hpp"
#include <fst/event.h>

namespace engine {
namespace event {
	//	enum : fst::evt::Id {
	//		OnDownKeyDown,
	//		OnUpKeyDown,
	//		OnLeftKeyDown,
	//		OnRightKeyDown
	//	};
	//	struct DownKeyDown {
	//		void (*func)(void* data);
	//		void* data;
	//
	//		void Call()
	//		{
	//			func(data);
	//		}
	//	};
	//
	//	struct UpKeyDown {
	//		void (*func)(void* data);
	//		void* data;
	//
	//		void Call()
	//		{
	//			func(data);
	//		}
	//	};
	//
	//	struct LeftKeyDown {
	//		void (*func)(void* data);
	//		void* data;
	//
	//		void Call()
	//		{
	//			func(data);
	//		}
	//	};
	//
	//	struct RightKeyDown {
	//		void (*func)(void* data);
	//		void* data;
	//
	//		void Call()
	//		{
	//			func(data);
	//		}
	//	};

	class Manager {
	public:
		Manager(fst::evt::dispatcher<engine::Entity>* evt_dispatcher);
		~Manager() = default;

		void SetWindowHandle(void* handle);

		//		template <typename T> void Connect(const T& evt)
		//		{
		//			events.push_back(evt);
		//		}

		void PollEvents();

		// fst::multi_vector<DownKeyDown, UpKeyDown, RightKeyDown, LeftKeyDown>
		// events;
		fst::evt::dispatcher<engine::Entity>* _evt_dispatcher;

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
