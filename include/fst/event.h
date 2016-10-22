#pragma once

#include "def.h"
#include "print.h"
//#include "multi_vector.h"
//#include "multi_key_vector.h"
#include <map>
//#include <multimap>
#include <vector>

namespace fst {
namespace evt {
	typedef unsigned long Id;
	enum class priority { urgent, high, normal, low };

	template <int...> struct ArgumentSizeList {
	};

	template <int N, int... S> struct GenArgumentSizeList : GenArgumentSizeList<N - 1, N - 1, S...> {
	};

	template <int... S> struct GenArgumentSizeList<0, S...> {
		typedef ArgumentSizeList<S...> type;
	};

	class abstract_message {
	public:
		virtual ~abstract_message()
		{
		}
	};

	template <typename... Args> class message : public abstract_message {
	public:
		message(const Args&... args)
			: _params(args...)
		{
		}
		std::tuple<Args...> _params;
	};

	class abstract_function {
	public:
		virtual ~abstract_function()
		{
		}
		virtual void Call(std::shared_ptr<abstract_message> msg) = 0;
	};

	template <typename T, typename... Args> struct function_holder : public abstract_function {
		T func_ptr;

		function_holder(T func)
			: func_ptr(func)
		{
		}

		template <int... S> void callFunc(std::shared_ptr<message<Args...>> msg, ArgumentSizeList<S...>)
		{
			func_ptr(std::get<S>(msg->_params)...);
		}

		virtual void Call(std::shared_ptr<abstract_message> msg)
		{
			callFunc(std::static_pointer_cast<message<Args...>>(msg),
				typename GenArgumentSizeList<sizeof...(Args)>::type());
		}

		std::shared_ptr<abstract_function> Copy()
		{
			return std::shared_ptr<abstract_function>(new function_holder(func_ptr));
		}
	};

	template <typename... Args, typename T> function_holder<T, Args...> function(T fct)
	{
		return function_holder<T, Args...>(fct);
	}

	// Member function.
	template <typename Type, typename... Args> struct function_holder_with_type : public abstract_function {
		Type* type;
		void (Type::*func_ptr)(Args...);

		function_holder_with_type(Type* type_, void (Type::*func_)(Args...))
			: type(type_)
			, func_ptr(func_)
		{
		}

		void operator()(Args... args)
		{
			return ((type)->*(func_ptr))(args...);
		}

		std::shared_ptr<abstract_function> Copy()
		{
			return std::shared_ptr<abstract_function>(new function_holder_with_type(type, func_ptr));
		}

		template <int... S> void callFunc(std::shared_ptr<message<Args...>> msg, ArgumentSizeList<S...>)
		{
			((type)->*(func_ptr))(std::get<S>(msg->_params)...);
		}

		virtual void Call(std::shared_ptr<abstract_message> msg)
		{
			callFunc(std::static_pointer_cast<message<Args...>>(msg),
				typename GenArgumentSizeList<sizeof...(Args)>::type());
		}
	};

	template <typename Type, typename... Args>
	function_holder_with_type<Type, Args...> function(Type* data, void (Type::*fct)(Args...))
	{
		fst::print(ptrace);
		return function_holder_with_type<Type, Args...>(data, fct);
	}

	class binded_event {
	public:
		binded_event()
		{
		}

		binded_event(std::shared_ptr<abstract_function> fct, std::shared_ptr<abstract_message> msg)
			: _fct(fct)
			, _msg(msg)
		{
		}

		void call()
		{
			_fct->Call(_msg);
		}

	private:
		std::shared_ptr<abstract_function> _fct;
		std::shared_ptr<abstract_message> _msg;
	};

	template <typename... Args, typename Type, typename... MsgArgs>
	binded_event bind(function_holder_with_type<Type, Args...> fct, MsgArgs... msg)
	{
		return binded_event(fct.Copy(), std::shared_ptr<abstract_message>(new message<MsgArgs...>(msg...)));
	}

	template <typename Key, typename... Priorities> class dispatcher {
	public:
		dispatcher()
		{
		}

		template <typename... Args, typename Type, typename... MsgArgs>
		void connect(Key obj_id, evt::Id evt_id, function_holder_with_type<Type, Args...> fct)
		{
			auto it = _evt_map.find(obj_id);

			// Add if object id is found.
			if (it != _evt_map.end()) {
				it->second.insert(std::pair<Id, std::shared_ptr<abstract_function>>(evt_id, fct.Copy()));
			}
			else {
				// Create multimap for object id .
				std::multimap<Id, std::shared_ptr<abstract_function>> evt_map;
				evt_map.insert(std::pair<Id, std::shared_ptr<abstract_function>>(evt_id, fct.Copy()));

				// Insert multimap to object id .
				_evt_map.insert(
					std::pair<Key, std::multimap<Id, std::shared_ptr<abstract_function>>>(obj_id, evt_map));
			}
		}

		template <typename... Args, typename T, typename... MsgArgs>
		void connect(Key obj_id, evt::Id evt_id, function_holder<T, Args...> fct)
		{
			auto it = _evt_map.find(obj_id);

			// Add if object id is found.
			if (it != _evt_map.end()) {
				it->second.insert(std::pair<Id, std::shared_ptr<abstract_function>>(evt_id, fct.Copy()));
			}
			else {
				// Create multimap for object id .
				std::multimap<Id, std::shared_ptr<abstract_function>> evt_map;
				evt_map.insert(std::pair<Id, std::shared_ptr<abstract_function>>(evt_id, fct.Copy()));

				// Insert multimap to object id .
				_evt_map.insert(
					std::pair<Key, std::multimap<Id, std::shared_ptr<abstract_function>>>(obj_id, evt_map));
			}
		}

		// template<typename ...Args, typename T>
		template <typename... MsgArgs, typename T> void connect(Key obj_id, evt::Id evt_id, T func)
		{
			function_holder<T, MsgArgs...> fct(func);
			auto it = _evt_map.find(obj_id);

			// Add if object id is found.
			if (it != _evt_map.end()) {
				it->second.insert(std::pair<Id, std::shared_ptr<abstract_function>>(evt_id, fct.Copy()));
			}
			else {
				// Create multimap for object id .
				std::multimap<Id, std::shared_ptr<abstract_function>> evt_map;
				evt_map.insert(std::pair<Id, std::shared_ptr<abstract_function>>(evt_id, fct.Copy()));

				// Insert multimap to object id .
				_evt_map.insert(
					std::pair<Key, std::multimap<Id, std::shared_ptr<abstract_function>>>(obj_id, evt_map));
			}
		}

		template <typename... Args> void push_event(Key obj_id, evt::Id evt_id, Args... args)
		{
			// Search for object id in object map.
			auto it = _evt_map.find(obj_id);

			// If object id is not in map then do nothing.
			if (it == _evt_map.end()) {
				//					global_manager_mutex.unlock();
				return;
			}

			// Pair of the first and last element of this id.
			auto range(it->second.equal_range(evt_id));

			if (range.first == it->second.end()) {
				//					global_manager_mutex.unlock();
				return;
			}

			std::shared_ptr<abstract_message> msg(new message<Args...>(args...));

			// Add every connected functions to this event id to the event queue.
			for (auto& i = range.first; i != range.second; ++i) {
				// Add binded function to event queue.
				//    _evt_queue.push_back(BindedEvent(i->second, msg));
				_queue.emplace_back(binded_event(i->second, msg));
				//    _evt_queue.push(BindedEvent(i->second, msg, priority));
			}
		}

		void call_events()
		{
			for (auto& n : _queue) {
				n.call();
			}

			_queue.clear();
		}

		//			void remove_key_connection(Key obj_id) {
		//
		//			}
		//
		//			void remove_event_connection(Key obj_id, evt::Id evt_id) {
		//
		//			}

	private:
		//			multi_vector<std::shared_ptr<binded_event>> _queue;
		std::vector<binded_event> _queue;
		std::map<Key, std::multimap<Id, std::shared_ptr<abstract_function>>> _evt_map;
	};

} // evt.
} // fst.
