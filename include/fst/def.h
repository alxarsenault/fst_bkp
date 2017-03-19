#pragma once

#define __FST_CONFIG_USE_STDLIB__ (1)

#define _FST_UNUSED __attribute__((unused))

#define _FST_ALWAYS_INLINE __attribute__((__always_inline__))

#define _FST_HIDDEN_INLINE __attribute__((__visibility__("hidden"), __always_inline__))

/// @todo Move this to type traits.
namespace fst {
template <bool>
struct enable_if {
};
template <>
struct enable_if<true> {
	typedef int type;
};
template <>
struct enable_if<false> {
};
}

#define FST_TRUE(x) typename fst::enable_if<x>::type = 0
#define FST_FALSE(x) typename fst::enable_if<!x>::type = 0

#if __FST_CONFIG_USE_STDLIB__ == 1
#define __FST_USE_STDLIB__
#endif // __FST_USE_STDLIB__
