#include "fst/print.h"
#include "fst/span.h"
#include <array>
#include <unordered_map>
#include <thread>
#include <memory>
#include <vector>

#include <new>

// Replace new and delete just for the purpose of demonstrating that
//  they are not called.

std::size_t memory = 0;
std::size_t alloc = 0;

void* operator new(std::size_t s) {
  memory += s;
  ++alloc;
  return malloc(s);
}

void operator delete(void* p) throw() {
  fst::print("---alloc", alloc, p);
  --alloc;
  free(p);
}

void memuse() {
  std::cout << "memory = " << memory << '\n';
  std::cout << "alloc = " << alloc << '\n';
}

namespace fst {
template <typename T>
static T& get_thread_local_instance() // no thread_local needed here
    noexcept(std::is_nothrow_constructible<T>::value) {
  // Guaranteed to be destroyed.
  // Instantiated on first use.
  // Thread safe in C++11.
  thread_local static T instance;
  return instance;
}

// template <class T, std::size_t N, class Allocator = std::allocator<T>>
// class stack_allocator
//{
//	public:
//
//	typedef typename std::allocator_traits<Allocator>::value_type value_type;
//	typedef typename std::allocator_traits<Allocator>::pointer pointer;
//	typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
//	typedef typename Allocator::reference reference;
//	typedef typename Allocator::const_reference const_reference;
//	typedef typename std::allocator_traits<Allocator>::size_type size_type;
//	typedef typename std::allocator_traits<Allocator>::difference_type difference_type;
//
//	typedef typename std::allocator_traits<Allocator>::const_void_pointer const_void_pointer;
//	typedef Allocator allocator_type;
//
//	public:
//
//	explicit stack_allocator(const allocator_type& alloc = allocator_type())
//		: m_allocator(alloc), m_begin(nullptr), m_end(nullptr), m_stack_pointer(nullptr)
//	{ }
//
//	explicit stack_allocator(pointer buffer, const allocator_type& alloc = allocator_type())
//		: m_allocator(alloc), m_begin(buffer), m_end(buffer + N),
//			m_stack_pointer(buffer)
//	{ }
//
//	template <class U>
//	stack_allocator(const stack_allocator<U, N, Allocator>& other)
//		: m_allocator(other.m_allocator), m_begin(other.m_begin), m_end(other.m_end),
//			m_stack_pointer(other.m_stack_pointer)
//	{ }
//
//	constexpr static size_type capacity()
//	{
//		return N;
//	}
//
//	pointer allocate(size_type n, const_void_pointer hint = const_void_pointer())
//	{
//		if (n <= size_type(std::distance(m_stack_pointer, m_end)))
//		{
//			pointer result = m_stack_pointer;
//			m_stack_pointer += n;
//			return result;
//		}
//
//		return m_allocator.allocate(n, hint);
//	}
//
//	void deallocate(pointer p, size_type n)
//	{
//		if (pointer_to_internal_buffer(p))
//		{
//			m_stack_pointer -= n;
//		}
//		else m_allocator.deallocate(p, n);
//	}
//
//	size_type max_size() const noexcept
//	{
//		return m_allocator.max_size();
//	}
//
//	template <class U, class... Args>
//	void construct(U* p, Args&&... args)
//	{
//		m_allocator.construct(p, std::forward<Args>(args)...);
//	}
//
//	template <class U>
//	void destroy(U* p)
//	{
//		m_allocator.destroy(p);
//	}
//
//	pointer address(reference x) const noexcept
//	{
//		if (pointer_to_internal_buffer(std::addressof(x)))
//		{
//			return std::addressof(x);
//		}
//
//		return m_allocator.address(x);
//	}
//
//	const_pointer address(const_reference x) const noexcept
//	{
//		if (pointer_to_internal_buffer(std::addressof(x)))
//		{
//			return std::addressof(x);
//		}
//
//		return m_allocator.address(x);
//	}
//
//	template <class U>
//	struct rebind { typedef stack_allocator<U, N, allocator_type> other; };
//
//	pointer buffer() const noexcept
//	{
//		return m_begin;
//	}
//
//	private:
//
//	bool pointer_to_internal_buffer(const_pointer p) const
//	{
//		return (!(std::less<const_pointer>()(p, m_begin)) && (std::less<const_pointer>()(p, m_end)));
//	}
//
//	allocator_type m_allocator;
//	pointer m_begin;
//	pointer m_end;
//	pointer m_stack_pointer;
//};
//
// template <class T1, std::size_t N, class Allocator, class T2>
// bool operator == (const stack_allocator<T1, N, Allocator>& lhs,
//	const stack_allocator<T2, N, Allocator>& rhs) noexcept
//{
//	return lhs.buffer() == rhs.buffer();
//}
//
// template <class T1, std::size_t N, class Allocator, class T2>
// bool operator != (const stack_allocator<T1, N, Allocator>& lhs,
//	const stack_allocator<T2, N, Allocator>& rhs) noexcept
//{
//	return !(lhs == rhs);
//}
//
//// -------- Specialization for void
////
// template <std::size_t N, class Allocator>
// class stack_allocator<void, N, Allocator>
//{
//	public:
//
//	typedef std::size_t size_type;
//	typedef std::ptrdiff_t difference_type;
//	typedef void* pointer;
//	typedef const void* const_pointer;
//	typedef void value_type;
//
//	constexpr pointer buffer() const noexcept
//	{
//		return nullptr;
//	}
//
//	template <class U>
//	struct rebind
//	{
//		typedef stack_allocator<U, N, typename Allocator::template rebind<U>::other> other;
//	};
//};

template <class T>
class stack_allocator {
  template <typename AA>
  friend class stack_allocator;

public:
  using Allocator = std::allocator<T>;
  typedef typename std::allocator_traits<Allocator>::value_type value_type;
  typedef typename std::allocator_traits<Allocator>::pointer pointer;
  typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
  typedef typename Allocator::reference reference;
  typedef typename Allocator::const_reference const_reference;
  typedef typename std::allocator_traits<Allocator>::size_type size_type;
  typedef typename std::allocator_traits<Allocator>::difference_type difference_type;

  typedef typename std::allocator_traits<Allocator>::const_void_pointer const_void_pointer;
  typedef Allocator allocator_type;

public:
  explicit stack_allocator(const allocator_type& alloc = allocator_type())
      : m_allocator(alloc)
      , m_begin(nullptr)
      , m_end(nullptr)
      , m_stack_pointer(nullptr) {}

  explicit stack_allocator(fst::span<T> buffer, const allocator_type& alloc = allocator_type())
      : m_allocator(alloc)
      , m_begin(buffer.begin())
      , m_end(buffer.end())
      , m_stack_pointer(buffer.begin()) {}

  template <class U>
  stack_allocator(const stack_allocator<U>& other)
      : m_allocator(other.m_allocator)
      , m_begin((pointer)other.m_begin)
      , m_end((pointer)other.m_end)
      , m_stack_pointer((pointer)other.m_stack_pointer) {
    fst::print("FDLJK", (std::size_t)m_begin, (std::size_t)m_stack_pointer, std::distance(m_stack_pointer, m_end),
        m_stack_pointer - m_begin);
  }

  //	constexpr static size_type capacity()
  //	{
  //		return 0;
  //	}

  pointer allocate(size_type n, const_void_pointer hint = const_void_pointer()) {
    fst::print("allocate", n, m_stack_pointer - m_begin);
    if (n <= size_type(std::distance(m_stack_pointer, m_end))) {
      pointer result = m_stack_pointer;
      m_stack_pointer += n;
      fst::print("allocate--", m_stack_pointer - m_begin);
      return result;
    }

    return m_allocator.allocate(n, hint);
  }

  void deallocate(pointer p, size_type n) {
    fst::print("deallocate", n);
    if (pointer_to_internal_buffer(p)) {
      m_stack_pointer -= n;
    }
    else
      m_allocator.deallocate(p, n);
  }

  size_type max_size() const noexcept { return m_allocator.max_size(); }

  template <class U, class... Args>
  void construct(U* p, Args&&... args) {
    m_allocator.construct(p, std::forward<Args>(args)...);
  }

  template <class U>
  void destroy(U* p) {
    m_allocator.destroy(p);
  }

  pointer address(reference x) const noexcept {
    if (pointer_to_internal_buffer(std::addressof(x))) {
      return std::addressof(x);
    }

    return m_allocator.address(x);
  }

  const_pointer address(const_reference x) const noexcept {
    if (pointer_to_internal_buffer(std::addressof(x))) {
      return std::addressof(x);
    }

    return m_allocator.address(x);
  }

  template <class U>
  struct rebind {
    typedef stack_allocator<U> other;
  };

  pointer buffer() const noexcept { return m_begin; }

private:
  bool pointer_to_internal_buffer(const_pointer p) const {
    return (!(std::less<const_pointer>()(p, m_begin)) && (std::less<const_pointer>()(p, m_end)));
  }

  allocator_type m_allocator;
  pointer m_begin;
  pointer m_end;
  pointer m_stack_pointer;
};

template <class T1, class T2>
bool operator==(const stack_allocator<T1>& lhs, const stack_allocator<T2>& rhs) noexcept {
  return lhs.buffer() == rhs.buffer();
}

template <class T1, class T2>
bool operator!=(const stack_allocator<T1>& lhs, const stack_allocator<T2>& rhs) noexcept {
  return !(lhs == rhs);
}
} // namespace fst.

int main(int argc, char** argv) {
  constexpr std::size_t m_size = 64 * sizeof(int);
  using memory_buffer = std::array<std::uint8_t, m_size>;
  memory_buffer& mb = fst::get_thread_local_instance<memory_buffer>();

  std::vector<int, fst::stack_allocator<int>> vec((fst::stack_allocator<int>(fst::stack_allocator<std::uint8_t>(mb))));

  vec.push_back(32);
  vec.push_back(33);
  vec.push_back(34);

  fst::print(sizeof(int), sizeof(float));

  std::vector<float, fst::stack_allocator<float>> dvec((fst::stack_allocator<float>(vec.get_allocator())));

  dvec.push_back(42);
  dvec.push_back(43);
  dvec.push_back(44);
  memuse();

  fst::print(vec[0], vec[1], vec[2]);
  fst::print(dvec[0], dvec[1], dvec[2]);
  return 0;
}

// int main(int argc, char** argv) {
//  constexpr std::size_t m_size = 64 * sizeof(int);
//  using memory_buffer = std::array<std::uint8_t, m_size>;
//  memory_buffer& mb = fst::get_thread_local_instance<memory_buffer>();
//  using s_allocator = fst::stack_allocator<int>;
//
//  fst::stack_allocator<std::uint8_t> allocator(mb);
////  s_allocator bb(allocator);
//
//  std::vector<int, fst::stack_allocator<int>> vec((fst::stack_allocator<int>(allocator)));
//  vec.resize(10);
//  vec[0] = 86;
//  fst::print(vec[0]);
////  std::vector<int, s_allocator> vvec((s_allocator(allocator)));
//
//
////  using s_allocator = fst::stack_allocator<int, m_size / sizeof(int)>;
////  std::vector<int, s_allocator> ab((fst::stack_allocator<int, 32>(mb.data())));
//
////  std::vector<int, s_allocator> vec((s_allocator(fst::span<int>((int*) mb.data(), m_size / sizeof(int)))));
////  vec.resize(64);
////  vec[0] = 86;
////
////  using c_allocator = fst::stack_allocator<char>;
////  int hj = 32;
////  std::vector<int, fst::stack_allocator<int>> kkk;
////  vec = decltype(vec)();
////  std::vector<int, fst::stack_allocator<int>>().swap(vec);
////  std::vector<int, fst::stack_allocator<int>> vec2;
//  std::vector<char, fst::stack_allocator<char>> cvec((fst::stack_allocator<char>(vec.get_allocator())));
//  cvec.resize(10);
//  cvec[0] = 78;
////
//   fst::print(vec[0], vec.size());
//   memuse();
//
////   memuse();
//  std::vector<int, fst::stack_allocator<int>>(fst::stack_allocator<int>(vec.get_allocator())).swap(vec);
////  memuse();
//
//  fst::print((int)cvec[0]);
//  memuse();
//  return 0;
//}
