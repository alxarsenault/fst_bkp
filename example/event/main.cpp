#include <fst/print.h>
#include <fst/event.h>
#include <string>
class Test1 {
public:
	Test1()
	{
	}
	void Bang()
	{
		fst::print("Bang1");
	}
	void Soccer(int a, int b)
	{
		fst::print("Soccer1", a, b);
	}
};

class Test2 {
public:
	Test2()
	{
	}
	void Bang()
	{
		fst::print("Bang2");
	}
	void Soccer(int a, int b)
	{
		fst::print("Soccer2", a, b);
	}
};

using Entity = long;

struct Button {
	Entity entity;
	fst::evt::Id onclick = 0;
};

struct Object {
	Entity entity;

	template <typename... Args>
	void push_event(fst::evt::Id id, Args... args)
	{
		push_event(entity, id);
	}
};

struct Slider : public Object {
	fst::evt::Id onclick = 0;
	fst::evt::Id onFuck = 1;
};

int main()
{
	//	Button a{1};
	//	Button b{2};
	//	Slider sld;
	//	sld.entity = 39783;
	//
	//
	////	fst::evt::Id my_entity_soccer_evt = 89;
	////	fst::evt::Id my_entity_bang_evt = 90;
	//	Test1 t1;
	//	Test2 t2;
	//
	//
	//
	//	fst::evt::dispatcher<Entity> dispatcher;
	//	dispatcher.connect(a.entity, b.onclick, fst::evt::function(&t1, &Test1::Bang));
	//	dispatcher.connect(a.entity, b.onclick, fst::evt::function(&t2, &Test2::Bang));
	//
	//	dispatcher.connect(b.entity, a.onclick, fst::evt::function(&t2, &Test2::Bang));
	//	dispatcher.connect(b.entity, a.onclick, fst::evt::function(&t2, &Test2::Bang));

	fst::evt::dispatcher<Entity> dispatcher;
	//	dispatcher.connect(30, 3, fst::evt::function<int, int>([](int a, int b) {
	//		fst::print(a, b);
	//	}));

	dispatcher.connect(
		34, 3, fst::evt::function<int, std::string>([](int a, std::string b) { fst::print(a, b); }));

	dispatcher.connect(
		34, 3, fst::evt::function<int, const char*>([](int a, const char* b) { fst::print(a, b); }));

	//	dispatcher.push_event(30, 3, 56, 64);
	dispatcher.push_event(34, 3, 34, std::string("abnbs"));
	//	dispatcher.push_event(30, 3);
	dispatcher.call_events();

	//	engine_core->component.push_back(
	//									 entity_info.entity, game::component::ZRotator(&engine_core->data,
	// entity_info.entity,
	//-0.3));

	//	engine_core->evt_dispatcher.connect(/
	//										0, engine::event::keyboard::OnUpKeyDown, fst::evt::function(this,
	//&Player::OnUpKeyDown));

	//	evt_manager->_evt_dispatcher->push_event(0, keyboard::OnDownKeyDown);

	//	fst::evt::dispatcher<Entity> dispatcher;
	//	dispatcher.add_connection(my_entity, my_entity_soccer_evt, fst::evt::function(&t1, &Test1::Soccer));
	//	dispatcher.add_connection(my_entity, my_entity_soccer_evt, fst::evt::function(&t2, &Test2::Soccer));
	//	dispatcher.add_connection(my_entity, my_entity_soccer_evt,
	//		fst::evt::function<int, int>([](int a, int b) { fst::print("Cunt :", a, b); }));
	//
	//	dispatcher.add_connection<int, int>(
	//		my_entity, my_entity_soccer_evt, [](int a, int b) { fst::print("Boom :", a, b); });
	//
	//	dispatcher.add_connection(my_entity, my_entity_bang_evt, fst::evt::function(&t1, &Test1::Bang));
	//	dispatcher.add_connection(my_entity, my_entity_bang_evt, fst::evt::function(&t2, &Test2::Bang));
	//	dispatcher.add_connection(
	//		my_entity, my_entity_bang_evt, fst::evt::function([]() { fst::print("Cunt"); }));
	//
	//	dispatcher.push_event(my_entity, my_entity_soccer_evt, 40, 45);
	//	dispatcher.push_event(my_entity, my_entity_bang_evt);
	//	dispatcher.call_events();

	return 0;
}
