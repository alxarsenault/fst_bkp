#pragma once

#include "def.h"
#include "print.h"
#include <stdlib.h>

#ifdef __FST_USE_STDLIB__
#define FST_ASSERT(x) ((x) ? (void)0 : (_FST_INTERNAL_ASSERT(x), abort()))
#define _FST_INTERNAL_ASSERT(x) fst::errprint('{', #x, '}', ptrace)

#define FST_ASSERT_MSG(x, str) ((x) ? (void)0 : (_FST_INTERNAL_ASSERT_MSG(x, str), abort()))
#define _FST_INTERNAL_ASSERT_MSG(x, str) fst::errprint(str, '{', #x, '}', ptrace)

#define FST_ASSERT_MSG_ONLY(x, str) ((x) ? (void)0 : (_FST_INTERNAL_ASSERT_MSG_ONLY(str), abort()))
#define _FST_INTERNAL_ASSERT_MSG_ONLY(str) fst::errprint(str, ptrace)

#else
#define FST_ASSERT(x, str)
#endif // __FST_USE_STDLIB__
