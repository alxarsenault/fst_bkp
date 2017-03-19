#include <fst/print.h>

#include <iostream>
#include <vector> // TODO: fst::vector doesn't compile.
//#include <tuple>
#include <experimental/tuple> // incoming in c++17
#include <type_traits>

// using std::experimental::apply;

auto mul = [](const auto& lhs, const auto& rhs) { return lhs * rhs; };
auto add = [](const auto& lhs, const auto& rhs) { return lhs + rhs; };
auto return_two_values = [](const auto& lhs, const auto& rhs) { return std::make_tuple(lhs, rhs); };

template <typename Computation, typename Output, typename Input>
struct node1 {
	Computation process;
	Input in_args;

	Output operator()()
	{
		return std::experimental::apply(process, in_args);
	}
};

template <typename Computation, typename... Args,
	typename Output = typename std::result_of<Computation&(Args...)>::type>
node1<Computation, Output, std::tuple<Args...>> make_node1(Computation c, Args... args)
{
	return node1<Computation, Output, std::tuple<Args...>>{ std::move(c), std::make_tuple(args...) };
}

template <typename Computation>
struct node2 {
	Computation process;

	template <typename... Args>
	auto operator()(const Args&... args)
	{
		return process(args...);
	}

	template <typename... Args>
	auto operator()(const std::tuple<Args...>& in_args)
	{
		return std::experimental::apply(process, in_args);
	}
};

template <typename Computation>
auto make_node2(Computation c)
{
	return node2<Computation>{ std::move(c) };
}

template <typename n1, typename n2, typename n3>
struct adapter {
	n1* in1;
	n2* in2;
	n3* out1;

	template <typename... Args1, typename... Args2>
	auto operator()(const std::tuple<Args1...>& in1_args, const std::tuple<Args2...>& in2_args)
	{
		return (*out1)((*in1)(in1_args), (*in2)(in2_args));
	}
};

template <typename N1, typename N2, typename N3>
auto make_adapter(N1* n1, N2* n2, N3* n3)
{
	return adapter<N1, N2, N3>{ n1, n2, n3 };
}

void lambda_storing_test()
{
	auto m_mul = [](const auto& lhs, const auto& rhs) { return lhs * rhs; };
	auto m_add = [](const auto& lhs, const auto& rhs) { return lhs + rhs; };

	// TODO: Replace with fst::vector (ordered multi-vec)?
	auto tuple = std::make_tuple(m_mul, m_add);

	int test = 42;
	fst::print(std::get<0>(tuple)(test, test), std::get<1>(tuple)(test, test));
}

int main(/*int argc, char* argv[]*/)
{
	/* Initial proof of concept. */
	lambda_storing_test();

	/* Store values at build-time. */
	auto n1 = make_node1(add, 42, 42);
	fst::print(n1());

	/* Pass values at execution. */
	auto n2 = make_node2(mul);
	fst::print(n2(42, 42));

	auto t = std::make_tuple(10, 10);
	fst::print(n2(t));

	int runtime_num;
	fst::print("please enter a number...");
	std::cin >> runtime_num;
	fst::print("answer (", runtime_num, "*", runtime_num, ") =", n2(runtime_num, runtime_num));

	/* Build graph. */
	auto node_mul = make_node2(mul);
	auto node_add = make_node2(add);
	auto adapter_adds = make_adapter(&node_add, &node_add, &node_add);
	fst::print(
		"adapter, (42 + 42) + (10 + 10) =", adapter_adds(std::make_tuple(42, 42), std::make_tuple(10, 10)));

	/* Can mix and match outputs and inputs with tuples. */
	auto node_two_v = make_node2(return_two_values);
	fst::print(node_mul(node_two_v(10, 6)));

	int result = node_mul(node_add(1, node_mul(2, 4)), node_mul(node_add(1, 1), node_add(2, 3)));
	fst::print("chaining, (1 + 2 * 4) * ((1 + 1) * (2 + 3)) =", result);

	/**
	 * CONCLUSION : This is insane, but cool.
	 *
	 * I forsee issues in representing this model. How do you get the number
	 * of arguments? And returns? You could use tuples for input and output,
	 * use their size to draw the node inputs/outputs... The problem is getting
	 * the size for the lambda arguments. But there is a way to do that in
	 * template declarations. But then again perf might die.
	 *
	 * Worth investigating a little more, probably unmanageable in the long run.
	 */
	return 0;
}
