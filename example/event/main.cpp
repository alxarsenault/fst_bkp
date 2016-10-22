#include <fst/print.h>
#include <fst/event.h>

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

int main()
{
	Entity my_entity = 0;
	fst::evt::Id my_entity_soccer_evt = 89;
	fst::evt::Id my_entity_bang_evt = 90;
	Test1 t1;
	Test2 t2;

	fst::evt::dispatcher<Entity> dispatcher;
	dispatcher.add_connection(my_entity, my_entity_soccer_evt, fst::evt::function(&t1, &Test1::Soccer));
	dispatcher.add_connection(my_entity, my_entity_soccer_evt, fst::evt::function(&t2, &Test2::Soccer));
	dispatcher.add_connection(my_entity, my_entity_soccer_evt,
		fst::evt::function<int, int>([](int a, int b) { fst::print("Cunt :", a, b); }));

	dispatcher.add_connection<int, int>(
		my_entity, my_entity_soccer_evt, [](int a, int b) { fst::print("Boom :", a, b); });

	dispatcher.add_connection(my_entity, my_entity_bang_evt, fst::evt::function(&t1, &Test1::Bang));
	dispatcher.add_connection(my_entity, my_entity_bang_evt, fst::evt::function(&t2, &Test2::Bang));
	dispatcher.add_connection(
		my_entity, my_entity_bang_evt, fst::evt::function([]() { fst::print("Cunt"); }));

	dispatcher.push_event(my_entity, my_entity_soccer_evt, 40, 45);
	dispatcher.push_event(my_entity, my_entity_bang_evt);
	dispatcher.call_events();

	return 0;
}
